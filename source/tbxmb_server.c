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
#include "microtbx.h"                            /* MicroTBX module                    */
#include "microtbxmodbus.h"                      /* MicroTBX-Modbus module             */
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
static void TbxMbServerFC04ReadInputReg(tTbxMbServerCtx       * context,
                                        tTbxMbTpPacket  const * rxPacket,
                                        tTbxMbTpPacket        * txPacket);


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
      newServerCtx->instancePtr = NULL;
      newServerCtx->pollFcn = NULL;
      newServerCtx->processFcn = TbxMbServerProcessEvent;
      newServerCtx->readInputFcn = NULL;
      newServerCtx->readCoilFcn = NULL;
      newServerCtx->writeCoilFcn = NULL;
      newServerCtx->readInputRegFcn = NULL;
      newServerCtx->readHoldingRegFcn = NULL;
      newServerCtx->writeHoldingRegFcn = NULL;
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
** \brief     Registers the callback function that this server calls, whenever a client
**            request the reading of a specific discrete input.
** \param     channel Handle to the Modbus server channel object.
** \param     callback Pointer to the callback function.
**
****************************************************************************************/
void TbxMbServerSetCallbackReadInput(tTbxMbServer          channel,
                                     tTbxMbServerReadInput callback)
{
  /* Verify parameters. */
  TBX_ASSERT((channel != NULL) && (callback != NULL));

  /* Only continue with valid parameters. */
  if ((channel != NULL) && (callback != NULL))
  {
    /* Convert the server channel pointer to the context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
    /* Store the callback function pointer. */
    TbxCriticalSectionEnter();
    serverCtx->readInputFcn = callback;
    TbxCriticalSectionExit();
  }
} /*** end of TbxMbServerSetCallbackReadInput ***/


/************************************************************************************//**
** \brief     Registers the callback function that this server calls, whenever a client
**            request the reading of a specific coil.
** \param     channel Handle to the Modbus server channel object.
** \param     callback Pointer to the callback function.
**
****************************************************************************************/
void TbxMbServerSetCallbackReadCoil(tTbxMbServer         channel,
                                    tTbxMbServerReadCoil callback)
{
  /* Verify parameters. */
  TBX_ASSERT((channel != NULL) && (callback != NULL));

  /* Only continue with valid parameters. */
  if ((channel != NULL) && (callback != NULL))
  {
    /* Convert the server channel pointer to the context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
    /* Store the callback function pointer. */
    TbxCriticalSectionEnter();
    serverCtx->readCoilFcn = callback;
    TbxCriticalSectionExit();
  }
} /*** end of TbxMbServerSetCallbackReadCoil ***/


/************************************************************************************//**
** \brief     Registers the callback function that this server calls, whenever a client
**            request the writing of a specific coil.
** \param     channel Handle to the Modbus server channel object.
** \param     callback Pointer to the callback function.
**
****************************************************************************************/
void TbxMbServerSetCallbackWriteCoil(tTbxMbServer          channel,
                                     tTbxMbServerWriteCoil callback)
{
  /* Verify parameters. */
  TBX_ASSERT((channel != NULL) && (callback != NULL));

  /* Only continue with valid parameters. */
  if ((channel != NULL) && (callback != NULL))
  {
    /* Convert the server channel pointer to the context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
    /* Store the callback function pointer. */
    TbxCriticalSectionEnter();
    serverCtx->writeCoilFcn = callback;
    TbxCriticalSectionExit();
  }
} /*** end of TbxMbServerSetCallbackWriteCoil ***/


/************************************************************************************//**
** \brief     Registers the callback function that this server calls, whenever a client
**            request the reading of a specific input register.
** \param     channel Handle to the Modbus server channel object.
** \param     callback Pointer to the callback function.
**
****************************************************************************************/
void TbxMbServerSetCallbackReadInputReg(tTbxMbServer             channel,
                                        tTbxMbServerReadInputReg callback)
{
  /* Verify parameters. */
  TBX_ASSERT((channel != NULL) && (callback != NULL));

  /* Only continue with valid parameters. */
  if ((channel != NULL) && (callback != NULL))
  {
    /* Convert the server channel pointer to the context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
    /* Store the callback function pointer. */
    TbxCriticalSectionEnter();
    serverCtx->readInputRegFcn = callback;
    TbxCriticalSectionExit();
  }
} /*** end of TbxMbServerSetCallbackReadInputReg ***/


/************************************************************************************//**
** \brief     Registers the callback function that this server calls, whenever a client
**            request the reading of a specific holding register.
** \param     channel Handle to the Modbus server channel object.
** \param     callback Pointer to the callback function.
**
****************************************************************************************/
void TbxMbServerSetCallbackReadHoldingReg(tTbxMbServer               channel,
                                          tTbxMbServerReadHoldingReg callback)
{
  /* Verify parameters. */
  TBX_ASSERT((channel != NULL) && (callback != NULL));

  /* Only continue with valid parameters. */
  if ((channel != NULL) && (callback != NULL))
  {
    /* Convert the server channel pointer to the context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
    /* Store the callback function pointer. */
    TbxCriticalSectionEnter();
    serverCtx->readHoldingRegFcn = callback;
    TbxCriticalSectionExit();
  }
} /*** end of TbxMbServerSetCallbackReadHoldingReg ***/


/************************************************************************************//**
** \brief     Registers the callback function that this server calls, whenever a client
**            request the writing of a specific holding register.
** \param     channel Handle to the Modbus server channel object.
** \param     callback Pointer to the callback function.
**
****************************************************************************************/
void TbxMbServerSetCallbackWriteHoldingReg(tTbxMbServer                channel,
                                           tTbxMbServerWriteHoldingReg callback)
{
  /* Verify parameters. */
  TBX_ASSERT((channel != NULL) && (callback != NULL));

  /* Only continue with valid parameters. */
  if ((channel != NULL) && (callback != NULL))
  {
    /* Convert the server channel pointer to the context structure. */
    tTbxMbServerCtx * serverCtx = (tTbxMbServerCtx *)channel;
    /* Sanity check on the context type. */
    TBX_ASSERT(serverCtx->type == TBX_MB_SERVER_CONTEXT_TYPE);
    /* Store the callback function pointer. */
    TbxCriticalSectionEnter();
    serverCtx->writeHoldingRegFcn = callback;
    TbxCriticalSectionExit();
  }
} /*** end of TbxMbServerSetCallbackWriteHoldingReg ***/


/************************************************************************************//**
** \brief     Helper function to extract an unsigned 16-bit value from the data of a 
**            Modbus packet. Note that unsigned 16-bit values are always store in the
**            big endian format, e.g. 0x1234 is stored as:
**            - data[0] = 0x12
**            - data[1] = 0x34
** \param     data Pointer to the byte array that holds the two bytes to extract, stored
**            in the big endian format.
** \return    The 16-bit unsigned integer value.
**
****************************************************************************************/
static inline uint16_t TbxMbServerExtractUInt16BE(uint8_t const * data)
{
  return ((uint16_t)data[0] << 8U) | data[1];
} /*** end of TbxMbServerExtractUInt16BE ***/


/************************************************************************************//**
** \brief     Helper function to store an unsigned 16-bit value in the data of a Modbus
**            packet. Note that unsigned 16-bit values are always stored in the
**            big endian format, e.g. 0x1234 is stored as:
**            - data[0] = 0x12
**            - data[1] = 0x34
** \param     value The unsigned 16-bit value to store.
** \param     data Pointer to the byte array where to store the value in the big endian
**            format.
**
****************************************************************************************/
static inline void TbxMbServerStoreUInt16BE(uint16_t   value,
                                            uint8_t  * data)
{
  data[0] = (uint8_t)(value >> 8U);
  data[1] = (uint8_t)value;
} /*** end of TbxMbServerExtractUInt16BE ***/


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
          uint8_t okayToSendResponse = TBX_FALSE;
          /* Obtain read access to the newly received packet and write access to the
           * response packet. 
           */
          tTbxMbTpPacket * rxPacket = serverCtx->tpCtx->getRxPacketFcn(serverCtx->tpCtx);
          tTbxMbTpPacket * txPacket = serverCtx->tpCtx->getTxPacketFcn(serverCtx->tpCtx);
          /* Since we're requested to process a newly received PDU, these packet accesses
           * should always succeed. Sanity check anyways, just in case.
           */
          TBX_ASSERT((rxPacket != NULL) && (txPacket != NULL));
          /* Only continue with packet access. */
          if ((rxPacket != NULL) && (txPacket != NULL))
          {
            /* Update flag that we can actually send a response, now that we know we 
             * have access to txPacket.
             */
            okayToSendResponse = TBX_TRUE;
            /* Prepare the response packet function code. */
            txPacket->pdu.code = rxPacket->pdu.code;
            /* Filter on the function code. */
            switch (rxPacket->pdu.code)
            {
              /* ---------------- FC04 - Read Input Register ------------------------- */
              case TBX_MB_FC04_READ_INPUT_REGISTER:
              {
                TbxMbServerFC04ReadInputReg(serverCtx, rxPacket, txPacket);
              }
              break;

              /* ---------------- Unsupported function code -------------------------- */
              default:
              {
                /* This function code is currently not supported. */
                txPacket->pdu.code |= TBX_MB_FC_EXCEPTION_MASK;
                txPacket->pdu.data[0] = TBX_MB_EC01_ILLEGAL_FUNCTION;
                txPacket->dataLen = 1U;
              }
              break;
            }
          }
          /* Inform the transport layer that were done with the rx packet and no longer
           * need access to it.
           */
          serverCtx->tpCtx->receptionDoneFcn(serverCtx->tpCtx);
          /* Request the transport layer to transmit the response. Note that
           * transmitFcn() should only be called after calling receptionDoneFcn().
           */
          if (okayToSendResponse == TBX_TRUE)
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


/************************************************************************************//**
** \brief     Handles a newly received PDU for function code 4 - Read Input Registers.
** \details   Note that this function is called at a time that txPacket->code is already
**            prepared. Also note that txPacket->node should not be touched here.
** \param     context Pointer to the Modbus server channel context.
** \param     rxPacket Received PDU packet with MUX access.
** \param     txPacket Storage for the PDU response packet with MUX access.
**
****************************************************************************************/
static void TbxMbServerFC04ReadInputReg(tTbxMbServerCtx       * context,
                                        tTbxMbTpPacket  const * rxPacket,
                                        tTbxMbTpPacket        * txPacket)
{
  /* Verify parameters. */
  TBX_ASSERT((context != NULL) && (rxPacket != NULL) && (txPacket != NULL));

  /* Only continue with valid parameters. */
  if ((context != NULL) && (rxPacket != NULL) && (txPacket != NULL))
  {
    /* Read out request packet parameters. */
    uint16_t startAddr = TbxMbServerExtractUInt16BE(&rxPacket->pdu.data[0]);
    uint16_t numRegs   = TbxMbServerExtractUInt16BE(&rxPacket->pdu.data[2]);

    /* Check if a callback function was registered. */
    if (context->readInputRegFcn == NULL)
    {
      /* Prepare exception response. */
      txPacket->pdu.code |= TBX_MB_FC_EXCEPTION_MASK;
      txPacket->pdu.data[0] = TBX_MB_EC01_ILLEGAL_FUNCTION;
      txPacket->dataLen = 1U;
    }
    /* Check if the quantity of registers is invalid. */
    else if ( (numRegs < 1U) || (numRegs > 125U) )
    {
      /* Prepare exception response. */
      txPacket->pdu.code |= TBX_MB_FC_EXCEPTION_MASK;
      txPacket->pdu.data[0] = TBX_MB_EC03_ILLEGAL_DATA_VALUE;
      txPacket->dataLen = 1U;
    }
    /* All is good for further processing. */
    else
    {
      /* Store byte count in the response and prepare the data lenght. */
      txPacket->pdu.data[0] = 2U * numRegs;
      txPacket->dataLen = txPacket->pdu.data[0] + 1U;
      /* Loop through all the registers. */
      for (uint8_t idx = 0U; idx < numRegs; idx++)
      {
        uint16_t regValue = 0U;
        tTbxMbServerResult srvResult;
        /* Obtain register value. */
        srvResult = context->readInputRegFcn(context, startAddr + idx, &regValue);
        /* No exception reported? */
        if (srvResult == TBX_MB_SERVER_OK)
        {
          /* Store the register value in the response. */
          TbxMbServerStoreUInt16BE(regValue, &txPacket->pdu.data[1U + (idx * 2U)]);
        }
        /* Exception detected. */
        else
        {
          /* Prepare exception response. */
          txPacket->pdu.code |= TBX_MB_FC_EXCEPTION_MASK;
          if (srvResult == TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR)
          {
            txPacket->pdu.data[0] = TBX_MB_EC02_ILLEGAL_DATA_ADDRESS;
          }
          else
          {
            txPacket->pdu.data[0] = TBX_MB_EC04_SERVER_DEVICE_FAILURE;
          }
          txPacket->dataLen = 1U;
          /* Stop looping. */
          break;
        }
      }
    }
  }
} /*** end of TbxMbServerFC04ReadInputReg ***/


/*********************************** end of tbxmb_server.c *****************************/
