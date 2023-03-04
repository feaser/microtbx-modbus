/************************************************************************************//**
* \file         tbxmb_server.c
* \brief        Modbus server source file.
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
#include "tbxmb_server_private.h"                /* MicroTBX-Modbus server private     */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Unique context type to identify a context as being a server channel. */
#define TBX_MB_SERVER_CONTEXT_TYPE     (37U)


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void TbxMbServerProcessEvent(tTbxMbEvent * event);


/************************************************************************************//**
** \brief     Creates a Modbus server channel object and assigs the specified Modbus
**            transport layer to the channel for packet transmission and reception.
** \param     transport Handle to a previously created Modbus transport layer object to
**            assign to the channel.
** \return    Handle to the newly created Modbus server channel object if successful,
**            NULL otherwise.
**
****************************************************************************************/
tTbxMbServer TbxMbServerCreate(tTbxMbTp transport)
{
  tTbxMbServer result = NULL;

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Allocate memory for the new channel context. */
    tTbxMbServerCtx * newServerCtx = TbxMemPoolAllocate(sizeof(tTbxMbServerCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (newServerCtx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbServerCtx));
      newServerCtx = TbxMemPoolAllocate(sizeof(tTbxMbServerCtx));      
    }
    /* Verify memory allocation of the channel context. */
    TBX_ASSERT(newServerCtx != NULL);
    /* Only continue if the memory allocation succeeded. */
    if (newServerCtx != NULL)
    {
      /* Convert the TP channel pointer to the context structure. */
      tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
      /* Sanity check on the transport layer's interface function. That way there is 
       * no need to do it later on, making it more run-time efficient. 
       */
      TBX_ASSERT((tpCtx->transmitFcn != NULL) && (tpCtx->receptionDoneFcn != NULL) &&
                 (tpCtx->getRxPacketFcn != NULL) && (tpCtx->getTxPacketFcn != NULL));
      /* Initialize the channel context. Start by crosslinking the transport layer. */
      newServerCtx->type = TBX_MB_SERVER_CONTEXT_TYPE;
      newServerCtx->pollFcn = NULL;
      newServerCtx->processFcn = TbxMbServerProcessEvent;
      newServerCtx->tpCtx = tpCtx;
      newServerCtx->tpCtx->channelCtx = newServerCtx;
      newServerCtx->tpCtx->isClient = TBX_FALSE;
      /* Update the result. */
      result = newServerCtx;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbServerCreate ****/


/************************************************************************************//**
** \brief     Releases a Modbus server channel object, previously created with
**            TbxMbServerCreate().
** \param     channel Handle to the Modbus server channel object to release.
**
****************************************************************************************/
void TbxMbServerFree(tTbxMbServer channel)
{
  /* Verify parameters. */
  TBX_ASSERT(channel != NULL);

  /* Only continue with valid parameters. */
  if (channel != NULL)
  {
    /* Convert the server channel pointer to the context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
    /* Remove crosslink between the channel and the transport layer. */
    TbxCriticalSectionEnter();
    serverCtx->tpCtx->channelCtx = NULL;
    serverCtx->tpCtx = NULL;
    /* Invalidate the context to protect it from accidentally being used afterwards. */
    serverCtx->type = 0U;
    serverCtx->pollFcn = NULL;
    serverCtx->processFcn = NULL;
    TbxCriticalSectionExit();
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(serverCtx);
  }
} /*** end of TbxMbServerFree ***/


/************************************************************************************//**
** \brief     Event processing function that is automatically called when an event for
**            this server channel object was received in TbxMbEventTask().
** \param     event Pointer to the event to process. Note that the event->context points
**            to the handle of the Modbus server channel object.
**
****************************************************************************************/
static void TbxMbServerProcessEvent(tTbxMbEvent * event)
{
  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* Sanity check the context. */
    TBX_ASSERT(event->context != NULL);
    /* Convert the event context to the server channel context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)event->context;
    /* Make sure the context is valid. */
    TBX_ASSERT(serverCtx != NULL);
    /* Only continue with a valid context. */
    if (serverCtx != NULL)
    {
      /* Sanity check on the context type. */
      TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
      /* Filter on the event identifier. */
      switch (event->id)
      {
        case TBX_MB_EVENT_ID_PDU_RECEIVED:
        {
          /* TODO Process the newly received PDU. 
           * - Use getRxPacketFcn() to access the rx packet.
           * - Use getTxPacketFcn() to access the tx packet and prepare the reponse.
           * - Use receptionDoneFcn() to release rx packet (and go to RTU IDLE).
           * - Use transmitFcn() to transmit the tx response packet (from RTU IDLE).
           * 
           * Current test code hardcodes the reading of the first input register.
           */
          tTbxMbTpPacket *rxPacket = serverCtx->tpCtx->getRxPacketFcn(serverCtx->tpCtx);
          tTbxMbTpPacket *txPacket = serverCtx->tpCtx->getTxPacketFcn(serverCtx->tpCtx);
          uint8_t sendReponse = TBX_FALSE;
          TBX_ASSERT(rxPacket != NULL);
          TBX_ASSERT(txPacket != NULL);
          /* Read discrete input register? */
          if (rxPacket->pdu.code == 4U)
          {
            uint16_t startAddr;
            uint16_t numRegs;
            startAddr = (uint16_t)(rxPacket->pdu.data[0] << 8U) | rxPacket->pdu.data[1];
            numRegs   = (uint16_t)(rxPacket->pdu.data[2] << 8U) | rxPacket->pdu.data[3];
            /* Reading just input register address 0 (element 1)? */
            if ( (startAddr == 0U) && (numRegs == 1U) )
            {
              txPacket->pdu.code = 4U;   
              txPacket->pdu.data[0] = 2U; /* Byte count. */
              txPacket->pdu.data[1] = 0x55U;
              txPacket->pdu.data[2] = 0xAAU;
              txPacket->dataLen = 3U;
              sendReponse = TBX_TRUE;
            }
          }
          /* Release rx packet (and go to RTU IDLE). */
          serverCtx->tpCtx->receptionDoneFcn(serverCtx->tpCtx);
          /* Transmit response? Should be done after receptionDoneFcn() otherwise
           * RTU is not in IDLE.
           */
          if (sendReponse == TBX_TRUE)
          {
            (void)serverCtx->tpCtx->transmitFcn(serverCtx->tpCtx);
          }
        }
        break;
      
        case TBX_MB_EVENT_ID_PDU_TRANSMITTED:
        {
          /* At this point no additional event handling is needed on this channel upon
           * PDU transmission completion.
           */
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
} /*** end of TbxMbServerProcessEvent ***/


/*********************************** end of tbxmb_server.c *****************************/
