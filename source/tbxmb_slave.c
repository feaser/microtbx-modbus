/************************************************************************************//**
* \file         tbxmb_slave.c
* \brief        Modbus slave source file.
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
#include "tbxmb_slave_private.h"                 /* MicroTBX-Modbus slave private      */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Unique context type to identify a context as being a slave channel. */
#define TBX_MB_SLAVE_CONTEXT_TYPE      (37U)


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void TbxMbSlaveProcessEvent(tTbxMbEvent * event);


/************************************************************************************//**
** \brief     Creates a Modbus slave channel object and assigs the specified Modbus
**            transport layer to the channel for packet transmission and reception.
** \param     transport Handle to a previously created Modbus transport layer object to
**            assign to the channel.
** \return    Handle to the newly created Modbus slave channel object if successful, NULL
**            otherwise.
**
****************************************************************************************/
tTbxMbSlave TbxMbSlaveCreate(tTbxMbTp transport)
{
  tTbxMbSlave result = NULL;

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Allocate memory for the new channel context. */
    tTbxMbSlaveCtx * newSlaveCtx = TbxMemPoolAllocate(sizeof(tTbxMbSlaveCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (newSlaveCtx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbSlaveCtx));
      newSlaveCtx = TbxMemPoolAllocate(sizeof(tTbxMbSlaveCtx));      
    }
    /* Verify memory allocation of the channel context. */
    TBX_ASSERT(newSlaveCtx != NULL);
    /* Only continue if the memory allocation succeeded. */
    if (newSlaveCtx != NULL)
    {
      /* Convert the TP channel pointer to the context structure. */
      tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
      /* Initialize the channel context. Start by crosslinking the transport layer. */
      newSlaveCtx->type = TBX_MB_SLAVE_CONTEXT_TYPE;
      newSlaveCtx->pollFcn = NULL;
      newSlaveCtx->processFcn = TbxMbSlaveProcessEvent;
      newSlaveCtx->tpCtx = tpCtx;
      newSlaveCtx->tpCtx->channelCtx = newSlaveCtx;
      newSlaveCtx->tpCtx->isMaster = TBX_FALSE;
      /* Update the result. */
      result = newSlaveCtx;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbSlaveCreate ****/


/************************************************************************************//**
** \brief     Releases a Modbus slave channel object, previously created with
**            TbxMbSlaveCreate().
** \param     channel Handle to the Modbus slave channel object to release.
**
****************************************************************************************/
void TbxMbSlaveFree(tTbxMbSlave channel)
{
  /* Verify parameters. */
  TBX_ASSERT(channel != NULL);

  /* Only continue with valid parameters. */
  if (channel != NULL)
  {
    /* Convert the slave channel pointer to the context structure. */
    tTbxMbSlaveCtx * slaveCtx = (tTbxMbSlaveCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(slaveCtx->type == TBX_MB_SLAVE_CONTEXT_TYPE);
    /* Remove crosslink between the channel and the transport layer. */
    TbxCriticalSectionEnter();
    slaveCtx->tpCtx->channelCtx = NULL;
    slaveCtx->tpCtx = NULL;
    /* Invalidate the context to protect it from accidentally being used afterwards. */
    slaveCtx->type = 0U;
    slaveCtx->pollFcn = NULL;
    slaveCtx->processFcn = NULL;
    TbxCriticalSectionExit();
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(slaveCtx);
  }
} /*** end of TbxMbSlaveFree ***/


/************************************************************************************//**
** \brief     Event processing function that is automatically called when an event for
**            this slave channel object was received in TbxMbEventTask().
** \param     event Pointer to the event to process. Note that the event->context points
**            to the handle of the Modbus slave channel object.
**
****************************************************************************************/
static void TbxMbSlaveProcessEvent(tTbxMbEvent * event)
{
  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* Sanity check the context. */
    TBX_ASSERT(event->context != NULL);
    /* Convert the event context to the slave channel context structure. */
    tTbxMbSlaveCtx * slaveCtx = (tTbxMbSlaveCtx *)event->context;
    /* Make sure the context is valid. */
    TBX_ASSERT(slaveCtx != NULL);
    /* Only continue with a valid context. */
    if (slaveCtx != NULL)
    {
      /* Sanity check on the context type. */
      TBX_ASSERT(slaveCtx->type == TBX_MB_SLAVE_CONTEXT_TYPE);
      /* Filter on the event identifier. */
      switch (event->id)
      {
        case TBX_MB_EVENT_ID_PDU_RECEIVED:
        {
          /* TODO Process the newly received PDU. */

          /* Inform the transport layer that we are done processing the PDU. */
          if (slaveCtx->tpCtx->receptionDoneFcn != NULL)
          {
            slaveCtx->tpCtx->receptionDoneFcn(slaveCtx->tpCtx);
          }
        }
        break;
      
        default:
        {
          /* An unsupported event was dispatched to us. Should not happen. */
          TBX_ASSERT(TBX_FALSE);
        }
        break;
      }


    }
  }
} /*** end of TbxMbSlaveProcessEvent ***/


/*********************************** end of tbxmb_slave.c ******************************/
