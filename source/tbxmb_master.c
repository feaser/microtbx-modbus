/************************************************************************************//**
* \file         tbxmb_master.c
* \brief        Modbus master source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2023 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
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
#include "microtbxmodbus.h"                      /* MicroTBX-Modbus module             */
#include "microtbx.h"                            /* MicroTBX module                    */
#include "tbxmb_checks.h"                        /* MicroTBX-Modbus config checks      */
#include "tbxmb_event_private.h"                 /* MicroTBX-Modbus event private      */
#include "tbxmb_tp_private.h"                    /* MicroTBX-Modbus TP private         */
#include "tbxmb_osal_private.h"                  /* MicroTBX-Modbus OSAL private       */
#include "tbxmb_master_private.h"                /* MicroTBX-Modbus master private     */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Unique context type to identify a context as being a master channel. */
#define TBX_MB_MASTER_CONTEXT_TYPE     (23U)


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void TbxMbMasterProcessEvent(tTbxMbEvent * event);


/************************************************************************************//**
** \brief     Creates a Modbus master channel object and assigns the specified Modbus
**            transport layer to the channel for packet transmission and reception.
** \param     transport Handle to a previously created Modbus transport layer object to
**            assign to the channel.
** \return    Handle to the newly created Modbus master channel object if successful,
**            NULL otherwise.
**
****************************************************************************************/
tTbxMbMaster TbxMbMasterCreate(tTbxMbTp transport)
{
  tTbxMbMaster result = NULL;

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Allocate memory for the new channel context. */
    tTbxMbMasterCtx * newMasterCtx = TbxMemPoolAllocate(sizeof(tTbxMbMasterCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (newMasterCtx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbMasterCtx));
      newMasterCtx = TbxMemPoolAllocate(sizeof(tTbxMbMasterCtx));      
    }
    /* Verify memory allocation of the channel context. */
    TBX_ASSERT(newMasterCtx != NULL);
    /* Only continue if the memory allocation succeeded. */
    if (newMasterCtx != NULL)
    {
      /* Convert the TP channel pointer to the context structure. */
      tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
      /* Initialize the channel context. Start by crosslinking the transport layer. */
      newMasterCtx->type = TBX_MB_MASTER_CONTEXT_TYPE;
      newMasterCtx->pollFcn = NULL;
      newMasterCtx->processFcn = TbxMbMasterProcessEvent;
      newMasterCtx->tpCtx = tpCtx;
      newMasterCtx->tpCtx->channelCtx = newMasterCtx;
      newMasterCtx->tpCtx->isMaster = TBX_TRUE;
      /* Update the result. */
      result = newMasterCtx;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbMasterCreate ****/


/************************************************************************************//**
** \brief     Releases a Modbus master channel object, previously created with
**            TbxMbSlaveCreate().
** \param     channel Handle to the Modbus master channel object to release.
**
****************************************************************************************/
void TbxMbMasterFree(tTbxMbMaster channel)
{
  /* Verify parameters. */
  TBX_ASSERT(channel != NULL);

  /* Only continue with valid parameters. */
  if (channel != NULL)
  {
    /* Convert the master channel pointer to the context structure. */
    tTbxMbMasterCtx * masterCtx = (tTbxMbMasterCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(masterCtx->type == TBX_MB_MASTER_CONTEXT_TYPE);
    /* Remove crosslink between the channel and the transport layer. */
    TbxCriticalSectionEnter();
    masterCtx->tpCtx->channelCtx = NULL;
    masterCtx->tpCtx = NULL;
    /* Invalidate the context to protect it from accidentally being used afterwards. */
    masterCtx->type = 0U;
    masterCtx->pollFcn = NULL;
    masterCtx->processFcn = NULL;
    TbxCriticalSectionExit();
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(masterCtx);
  }
} /*** end of TbxMbMasterFree ***/


/************************************************************************************//**
** \brief     Event processing function that is automatically called when an event for
**            this master channel object was received in TbxMbEventTask().
** \param     event Pointer to the event to process. Note that the event->context points
**            to the handle of the Modbus master channel object.
**
****************************************************************************************/
static void TbxMbMasterProcessEvent(tTbxMbEvent * event)
{
  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* Sanity check the context. */
    TBX_ASSERT(event->context != NULL);
    /* Convert the event context  to the master channel context structure. */
    tTbxMbMasterCtx * masterCtx = (tTbxMbMasterCtx *)event->context;
    /* Make sure the context is valid. */
    TBX_ASSERT(masterCtx != NULL);
    /* Only continue with a valid context. */
    if (masterCtx != NULL)
    {
      /* Sanity check on the context type. */
      TBX_ASSERT(masterCtx->type == TBX_MB_MASTER_CONTEXT_TYPE);
      /* TODO Implement TbxMbMasterProcessEvent(). */
      masterCtx->processFcn = TbxMbMasterProcessEvent; /* Dummy for now. */
    }
  }
} /*** end of TbxMbSlaveProcessEvent ***/


/*********************************** end of tbxmb_master.c *****************************/
