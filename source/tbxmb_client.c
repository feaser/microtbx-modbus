/************************************************************************************//**
* \file         tbxmb_client.c
* \brief        Modbus client source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2023 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
*
* SPDX-License-Identifier: GPL-3.0-or-later
*
* This file is part of MicroTBX-Modbus. MicroTBX-Modbus is free software: you can
* redistribute it and/or modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* MicroTBX-Modbus is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License along with MicroTBX-Modbus.
* If not, see www.gnu.org/licenses/.
*
* \endinternal
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "microtbx.h"                            /* MicroTBX module                    */
#include "microtbxmodbus.h"                      /* MicroTBX-Modbus module             */
#include "tbxmb_checks.h"                        /* MicroTBX-Modbus config checks      */
#include "tbxmb_event_private.h"                 /* MicroTBX-Modbus event private      */
#include "tbxmb_tp_private.h"                    /* MicroTBX-Modbus TP private         */
#include "tbxmb_osal_private.h"                  /* MicroTBX-Modbus OSAL private       */
#include "tbxmb_client_private.h"                /* MicroTBX-Modbus client private     */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Unique context type to identify a context as being a client channel. */
#define TBX_MB_CLIENT_CONTEXT_TYPE     (23U)


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void TbxMbClientProcessEvent(tTbxMbEvent * event);


/************************************************************************************//**
** \brief     Creates a Modbus client channel object and assigns the specified Modbus
**            transport layer to the channel for packet transmission and reception.
** \param     transport Handle to a previously created Modbus transport layer object to
**            assign to the channel.
** \return    Handle to the newly created Modbus client channel object if successful,
**            NULL otherwise.
**
****************************************************************************************/
tTbxMbClient TbxMbClientCreate(tTbxMbTp transport)
{
  tTbxMbClient result = NULL;

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Allocate memory for the new channel context. */
    tTbxMbClientCtx * newClientCtx = TbxMemPoolAllocate(sizeof(tTbxMbClientCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (newClientCtx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbClientCtx));
      newClientCtx = TbxMemPoolAllocate(sizeof(tTbxMbClientCtx));      
    }
    /* Verify memory allocation of the channel context. */
    TBX_ASSERT(newClientCtx != NULL);
    /* Only continue if the memory allocation succeeded. */
    if (newClientCtx != NULL)
    {
      /* Convert the TP channel pointer to the context structure. */
      tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
      /* Initialize the channel context. Start by crosslinking the transport layer. */
      newClientCtx->type = TBX_MB_CLIENT_CONTEXT_TYPE;
      newClientCtx->instancePtr = NULL;
      newClientCtx->pollFcn = NULL;
      newClientCtx->processFcn = TbxMbClientProcessEvent;
      newClientCtx->tpCtx = tpCtx;
      newClientCtx->tpCtx->channelCtx = newClientCtx;
      newClientCtx->tpCtx->isClient = TBX_TRUE;
      /* Update the result. */
      result = newClientCtx;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbClientCreate ****/


/************************************************************************************//**
** \brief     Releases a Modbus client channel object, previously created with
**            TbxMbClientCreate().
** \param     channel Handle to the Modbus client channel object to release.
**
****************************************************************************************/
void TbxMbClientFree(tTbxMbClient channel)
{
  /* Verify parameters. */
  TBX_ASSERT(channel != NULL);

  /* Only continue with valid parameters. */
  if (channel != NULL)
  {
    /* Convert the client channel pointer to the context structure. */
    tTbxMbClientCtx * clientCtx = (tTbxMbClientCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(clientCtx->type == TBX_MB_CLIENT_CONTEXT_TYPE);
    /* Remove crosslink between the channel and the transport layer. */
    TbxCriticalSectionEnter();
    clientCtx->tpCtx->channelCtx = NULL;
    clientCtx->tpCtx = NULL;
    /* Invalidate the context to protect it from accidentally being used afterwards. */
    clientCtx->type = 0U;
    clientCtx->pollFcn = NULL;
    clientCtx->processFcn = NULL;
    TbxCriticalSectionExit();
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(clientCtx);
  }
} /*** end of TbxMbClientFree ***/


/************************************************************************************//**
** \brief     Event processing function that is automatically called when an event for
**            this client channel object was received in TbxMbEventTask().
** \param     event Pointer to the event to process. Note that the event->context points
**            to the handle of the Modbus client channel object.
**
****************************************************************************************/
static void TbxMbClientProcessEvent(tTbxMbEvent * event)
{
  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* Sanity check the context. */
    TBX_ASSERT(event->context != NULL);
    /* Convert the event context  to the client channel context structure. */
    tTbxMbClientCtx * clientCtx = (tTbxMbClientCtx *)event->context;
    /* Make sure the context is valid. */
    TBX_ASSERT(clientCtx != NULL);
    /* Only continue with a valid context. */
    if (clientCtx != NULL)
    {
      /* Sanity check on the context type. */
      TBX_ASSERT(clientCtx->type == TBX_MB_CLIENT_CONTEXT_TYPE);
      /* TODO Implement TbxMbClientProcessEvent(). */
      clientCtx->processFcn = TbxMbClientProcessEvent; /* Dummy for now. */
    }
  }
} /*** end of TbxMbClientProcessEvent ***/


/*********************************** end of tbxmb_client.c *****************************/
