/************************************************************************************//**
* \file         tbxmb_rtu.c
* \brief        Modbus RTU transport layer source file.
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
#include "tbxmb_options.h"                       /* MicroTBX-Modbus config options     */
#include "microtbx.h"                            /* MicroTBX module                    */
#include "tbxmb_checks.h"                        /* MicroTBX-Modbus config checks      */
#include "microtbxmodbus.h"                      /* MicroTBX-Modbus module             */
#include "tbxmb_event_private.h"                 /* MicroTBX-Modbus event private      */
#include "tbxmb_tp_private.h"                    /* MicroTBX-Modbus TP private         */
#include "tbxmb_uart_private.h"                  /* MicroTBX-Modbus UART private       */
#include "tbxmb_osal_private.h"                  /* MicroTBX-Modbus OSAL private       */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Unique context type to identify a context as being an RTU transport layer. */
#define TBX_MB_RTU_CONTEXT_TYPE        (84U)


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void     TbxMbRtuPoll(tTbxMbTp transport);
static void     TbxMbRtuProcessEvent(tTbxMbEvent * event);
static uint8_t  TbxMbRtuTransmit(tTbxMbTp transport);
static uint8_t  TbxMbRtuValidate(tTbxMbTp transport);
static void     TbxMbRtuTransmitComplete(tTbxMbUartPort port);
static void     TbxMbRtuDataReceived(      tTbxMbUartPort   port, 
                                     const uint8_t        * data, 
                                           uint8_t          len);
static uint16_t TbxMbRtuCalculatCrc(const uint8_t  * data, 
                                          uint16_t   len);


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief RTU transport layer handle lookup table by UART port. Uses for finding the
 *         transport layer handle that uses a specific serial port, in a run-time
 *         efficient way.
 */
static tTbxMbTpCtx * tbxMbRtuCtx[TBX_MB_UART_NUM_PORT] = { 0 };


/************************************************************************************//**
** \brief     Creates a Modbus RTU transport layer object.
** \param     nodeAddr The address of the node. Can be in the range 1..247 for a slave
**            node. Set it to 0 for the master.
** \param     port The serial port to use. The actual meaning of the serial port is
**            hardware dependent. It typically maps to the UART peripheral number. E.g. 
**            TBX_MB_UART_PORT1 = USART1 on an STM32.
** \param     baudrate The desired communication speed.
** \param     stopbits Number of stop bits at the end of a character.
** \param     parity Parity bit type to use.
** \return    Handle to the newly created RTU transport layer object if successful, NULL
**            otherwise.
**
****************************************************************************************/
tTbxMbTp TbxMbRtuCreate(uint8_t            nodeAddr, 
                        tTbxMbUartPort     port, 
                        tTbxMbUartBaudrate baudrate,
                        tTbxMbUartStopbits stopbits,
                        tTbxMbUartParity   parity)
{
  tTbxMbTp result = NULL;

  /* Verify parameters. */
  TBX_ASSERT((nodeAddr <= 247U) &&
             (port < TBX_MB_UART_NUM_PORT) && 
             (baudrate < TBX_MB_UART_NUM_BAUDRATE) &&
             (stopbits < TBX_MB_UART_NUM_STOPBITS) &&
             (parity < TBX_MB_UART_NUM_PARITY));

  /* Only continue with valid parameters. */
  if ((nodeAddr <= 247U) &&
      (port < TBX_MB_UART_NUM_PORT) && 
      (baudrate < TBX_MB_UART_NUM_BAUDRATE) &&
      (stopbits < TBX_MB_UART_NUM_STOPBITS) &&
      (parity < TBX_MB_UART_NUM_PARITY))
  {
    /* Allocate memory for the new transport context. */
    tTbxMbTpCtx * newTpCtx = TbxMemPoolAllocate(sizeof(tTbxMbTpCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (newTpCtx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbTpCtx));
      newTpCtx = TbxMemPoolAllocate(sizeof(tTbxMbTpCtx));      
    }
    /* Verify memory allocation of the transport context. */
    TBX_ASSERT(newTpCtx != NULL);
    /* Only continue if the memory allocation succeeded. */
    if (newTpCtx != NULL)
    {
      /* Initialize the transport context. */
      newTpCtx->type = TBX_MB_RTU_CONTEXT_TYPE;
      newTpCtx->pollFcn = TbxMbRtuPoll;
      newTpCtx->processFcn = TbxMbRtuProcessEvent;
      newTpCtx->nodeAddr = nodeAddr;
      newTpCtx->port = port;
      newTpCtx->transmitFcn = TbxMbRtuTransmit;
      newTpCtx->txLocked = TBX_FALSE;
      newTpCtx->rxLocked = TBX_FALSE;
      /* Store the transport context in the lookup table. */
      tbxMbRtuCtx[port] = newTpCtx;
      /* Initialize the port. Note the RTU always uses 8 databits. */
      TbxMbUartInit(port, baudrate, TBX_MB_UART_8_DATABITS, stopbits, parity,
                    TbxMbRtuTransmitComplete, TbxMbRtuDataReceived);
      /* Update the result. */
      result = newTpCtx;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbRtuCreate ***/  


/************************************************************************************//**
** \brief     Releases a Modbus RTU transport layer object, previously created with 
**            TbxMbRtuCreate().
** \param     transport Handle to RTU transport layer object to release.
**
****************************************************************************************/
void TbxMbRtuFree(tTbxMbTp transport)
{
  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Convert the TP channel pointer to the context structure. */
    tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tpCtx->type == TBX_MB_RTU_CONTEXT_TYPE);
    /* Remove the channel from the lookup table. */
    tbxMbRtuCtx[tpCtx->port] = NULL;
    /* Invalidate the context to protect it from accidentally being used afterwards. */
    tpCtx->type = 0U;
    tpCtx->pollFcn = NULL;
    tpCtx->processFcn = NULL;
    /* Give the transport layer context back to the memory pool. */
    TbxMemPoolRelease(tpCtx);
  }
} /*** end of TbxMbRtuFree ***/


/************************************************************************************//**
** \brief     Event polling function that is automatically called during each call of
**            TbxMbEventTask(), if activate. Use the TBX_MB_EVENT_ID_START_POLLING and
**            TBX_MB_EVENT_ID_STOP_POLLING events to activate and deactivate.
** \param     transport Handle to RTU transport layer object.
**
****************************************************************************************/
static void TbxMbRtuPoll(tTbxMbTp transport)
{
  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Convert the TP channel pointer to the context structure. */
    tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tpCtx->type == TBX_MB_RTU_CONTEXT_TYPE);
    /* TODO Implement TbxMbRtuPoll(). */
    tpCtx->pollFcn = TbxMbRtuPoll; /* Dummy for now. */
  }
} /*** end of TbxMbRtuPoll ***/


/************************************************************************************//**
** \brief     Event processing function that is automatically called when an event for
**            this transport layer object was received in TbxMbEventTask().
** \param     event Pointer to the event to process. Note that the event->context points
**            to the handle of the RTU transport layer object.
**
****************************************************************************************/
static void TbxMbRtuProcessEvent(tTbxMbEvent * event)
{
  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* Sanity check the context. */
    TBX_ASSERT(event->context != NULL);
    /* Convert the event context to the TP context structure. */
    tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)event->context;
    /* Make sure the context is valid. */
    TBX_ASSERT(tpCtx != NULL);
    /* Only continue with a valid context. */
    if (tpCtx != NULL)
    {
      /* Sanity check on the context type. */
      TBX_ASSERT(tpCtx->type == TBX_MB_RTU_CONTEXT_TYPE);
      /* Filter on the event identifier. */
      switch (event->id)
      {
        case TBX_MB_EVENT_ID_PDU_RECEIVED:
        {
          /* Validate the newly received PDU at task level. */
          if (TbxMbRtuValidate(tpCtx) == TBX_OK)
          {
            /* The PDU is valid. Pass it on to the linked channel object for further 
             * processing.
            */
            tTbxMbEvent newEvent;
            newEvent.context = tpCtx->channelCtx;
            newEvent.id = TBX_MB_EVENT_ID_PDU_RECEIVED;
            TbxMbOsalPostEvent(&newEvent, TBX_FALSE);
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
} /*** end of TbxMbRtuProcessEvent ***/


/************************************************************************************//**
** \brief     Starts the transmission of a communication packet, stored in the transport
**            layer object.
** \param     transport Handle to RTU transport layer object.
** \return    TBX_OK if successful, TBX_ERROR otherwise. 
**
****************************************************************************************/
static uint8_t TbxMbRtuTransmit(tTbxMbTp transport)
{
  uint8_t result = TBX_ERROR;

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* TODO Still need to properly handle tp_crx->isMaster. In ECL++ Modbus I did a wait
   * loop to make sure INIT state is entered for the master.
   */

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Convert the TP channel pointer to the context structure. */
    tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tpCtx->type == TBX_MB_RTU_CONTEXT_TYPE);
    /* Check that no other packet transmission is already in progress. */
    uint8_t okayToTransmit = TBX_FALSE;
    TbxCriticalSectionEnter();
    if (tpCtx->txLocked == TBX_FALSE)
    {
      okayToTransmit = TBX_TRUE;
      /* Lock access to the tx_packet for the duration of the transmission. Note that
       * the unlock happens in TbxMbRtuTransmitComplete() of it the UART transmission
       * could not be started.
       */
      tpCtx->txLocked = TBX_TRUE;
    }
    TbxCriticalSectionExit();
    /* Only continue if no other packet transmission is already in progress. */
    if (okayToTransmit == TBX_TRUE)
    {
      /* Determine ADU specific properties. The ADU starts at one byte before the PDU, 
       * which is the last byte of head[]. The ADU's length is:
       * - Node address (1 byte)
       * - Function code (1 byte)
       * - Packet data (dataLen bytes)
       * - CRC16 (2 bytes)
       */
      uint8_t * aduPtr = &tpCtx->txPacket.head[TBX_MB_TP_ADU_HEAD_LEN_MAX-1U];
      uint16_t  aduLen = tpCtx->txPacket.dataLen + 4U;
      /* Populate the ADU head. For RTU it 
       * is the address field right in front of the
       * PDU. For master->slave transfers the address field is the slave's node address
       * (unicast) or 0 (broadcast). For slave-master transfers it always the slave's
       * node address.
       */
      aduPtr[0] = tpCtx->txPacket.node;
      /* Populate the ADU tail. For RTU it is the CRC16 right after the PDU's data. */
      uint16_t adu_crc = TbxMbRtuCalculatCrc(aduPtr, aduLen - 2U);
      aduPtr[aduLen - 2U] = (uint8_t)adu_crc;                         /* CRC16 low.  */
      aduPtr[aduLen - 1U] = (uint8_t)(adu_crc >> 8U);                 /* CRC16 high. */
      /* Pass ADU transmit request on to the UART module. */
      result = TbxMbUartTransmit(tpCtx->port, aduPtr, aduLen);
      /* Unlock access to the txPacket if the transmission could not be started. */
      if (result != TBX_OK)
      {
        TbxCriticalSectionEnter();
        tpCtx->txLocked = TBX_FALSE;
        TbxCriticalSectionExit();
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbRtuTransmit ***/


/************************************************************************************//**
** \brief     Validates a newly received communication packet, stored in the transport
**            layer object.
** \param     transport Handle to RTU transport layer object.
** \return    TBX_OK if successful, TBX_ERROR otherwise. 
**
****************************************************************************************/
static uint8_t TbxMbRtuValidate(tTbxMbTp transport)
{
  uint8_t result = TBX_ERROR;

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Convert the TP channel pointer to the context structure. */
    tTbxMbTpCtx * tpCtx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tpCtx->type == TBX_MB_RTU_CONTEXT_TYPE);
    /* TODO Implement TbxMbRtuValidate(). It needs to check the CRC of rxPacket. */
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbRtuValidate ***/


/************************************************************************************//**
** \brief     Event function to signal to this module that the entire transfer completed.
** \attention This function should be called by the UART module.
** \param     port The serial port that the transfer completed on.
**
****************************************************************************************/
static void TbxMbRtuTransmitComplete(tTbxMbUartPort port)
{
  /* Verify parameters. */
  TBX_ASSERT(port < TBX_MB_UART_NUM_PORT);

  /* Only continue with valid parameters. */
  if (port < TBX_MB_UART_NUM_PORT)
  {
    /* Obtain transport layer context linked to UART port of this event. */
    tTbxMbTpCtx * tpCtx = tbxMbRtuCtx[port];
    /* Verify transport layer context. */
    TBX_ASSERT(tpCtx != NULL)
    /* Only continue with a valid transport layer context. Note that there is not need
     * to also check the transport layer type, because only RTU types are stored in the
     * tbxMbRtuCtx[] array.
     */
    if (tpCtx != NULL)
    {
      /* Unlock access to the txPacket now that the transmission is complete. */
      TbxCriticalSectionEnter();
      tpCtx->txLocked = TBX_FALSE;
      TbxCriticalSectionExit();

      /* TODO Set an event for further handling at master/slave module task level. 
       * Probably want to pass the transport layer context to the event.
       * Not yet sure it this module needs its own task function. It might need it for
       * the packet reception state machine.
       * Perhaps it makes more sense to Unlock access to the txPacket at task level,
       * instead of the code right above here. Yep, because this function is called at
       * ISR level and that needs to be kept as short as possible.
       */
    }
  }
} /*** end of TbxMbRtuTransmitComplete ***/


/************************************************************************************//**
** \brief     Event function to signal the reception of new data to this module.
** \attention This function should be called by the UART module.
** \param     port The serial port that the transfer completed on.
** \param     data Byte array with newly received data.
** \param     len Number of newly received bytes.
**
****************************************************************************************/
static void TbxMbRtuDataReceived(      tTbxMbUartPort  port, 
                                 const uint8_t       * data, 
                                       uint8_t         len)
{
  /* Verify parameters. */
  TBX_ASSERT((port < TBX_MB_UART_NUM_PORT) && 
             (data != NULL) &&
             (len > 0U));

  /* Only continue with valid parameters. */
  if ((port < TBX_MB_UART_NUM_PORT) && 
      (data != NULL) &&
      (len > 0U))
  {
    /* TODO Implement TbxMbRtuDataReceived(). Use lookup table with port index to get
     * the channel handle (tbxMbRtuCtx[]). Probably need to set an event here to
     * further handle this event at task level. Note that at master/slave task level,
     * the validate() part still needs to be done.
     */
  }
} /*** end of TbxMbRtuDataReceived ***/


/************************************************************************************//**
** \brief     Calculates the Modbus RTU defined CRC16 checksum over the bytes in the
**            specified data array.
** \param     data Pointer to the byte array with data.
** \param     len Number of data bytes to include in the CRC16 calculation.
** \return    The calculated CRC16 checksum value.
**
****************************************************************************************/
static uint16_t TbxMbRtuCalculatCrc(const uint8_t  * data, 
                                          uint16_t   len)
{
  /* Lookup table for fast CRC16 calculation. Made static to lower the stack load. */
  static const uint16_t tbxMbRtuCrcTable[] =
  {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
  };
  uint16_t result = 0U;
  uint16_t crc = 0xFFFFU;

  /* Loop over all the data bytes. */
  for (uint16_t byteIdx = 0; byteIdx < len; byteIdx++)
  {
    /* Calculate the index into the CRC lookup table. */
    uint8_t tableIdx = data[byteIdx] ^ crc;
    crc >>= 8;
    crc ^= tbxMbRtuCrcTable[tableIdx];
  }
  /* Update the result. */
  result = crc;
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbRtuCalculatCrc ***/


/*********************************** end of tbxmb_rtu.c ********************************/
