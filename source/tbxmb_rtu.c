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
** \param     node_addr The address of the node. Can be in the range 1..247 for a slave
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
tTbxMbTp TbxMbRtuCreate(uint8_t            node_addr, 
                        tTbxMbUartPort     port, 
                        tTbxMbUartBaudrate baudrate,
                        tTbxMbUartStopbits stopbits,
                        tTbxMbUartParity   parity)
{
  tTbxMbTp result = NULL;

  /* Verify parameters. */
  TBX_ASSERT((node_addr <= 247U) &&
             (port < TBX_MB_UART_NUM_PORT) && 
             (baudrate < TBX_MB_UART_NUM_BAUDRATE) &&
             (stopbits < TBX_MB_UART_NUM_STOPBITS) &&
             (parity < TBX_MB_UART_NUM_PARITY));

  /* Only continue with valid parameters. */
  if ((node_addr <= 247U) &&
      (port < TBX_MB_UART_NUM_PORT) && 
      (baudrate < TBX_MB_UART_NUM_BAUDRATE) &&
      (stopbits < TBX_MB_UART_NUM_STOPBITS) &&
      (parity < TBX_MB_UART_NUM_PARITY))
  {
    /* Allocate memory for the new transport context. */
    tTbxMbTpCtx * new_tp_ctx = TbxMemPoolAllocate(sizeof(tTbxMbTpCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (new_tp_ctx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbTpCtx));
      new_tp_ctx = TbxMemPoolAllocate(sizeof(tTbxMbTpCtx));      
    }
    /* Verify memory allocation of the transport context. */
    TBX_ASSERT(new_tp_ctx != NULL)
    /* Only continue if the memory allocation succeeded. */
    if (new_tp_ctx != NULL)
    {
      /* Initialize the transport context. */
      new_tp_ctx->type = TBX_MB_RTU_CONTEXT_TYPE;
      new_tp_ctx->poll_fcn = TbxMbRtuPoll;
      new_tp_ctx->process_fcn = TbxMbRtuProcessEvent;
      new_tp_ctx->node_addr = node_addr;
      new_tp_ctx->port = port;
      new_tp_ctx->transmit_fcn = TbxMbRtuTransmit;
      new_tp_ctx->validate_fcn = TbxMbRtuValidate;
      new_tp_ctx->tx_in_progress = TBX_FALSE;
      new_tp_ctx->rx_in_progress = TBX_FALSE;
      /* Store the transport context in the lookup table. */
      tbxMbRtuCtx[port] = new_tp_ctx;
      /* Initialize the port. Note the RTU always uses 8 databits. */
      TbxMbUartInit(port, baudrate, TBX_MB_UART_8_DATABITS, stopbits, parity,
                    TbxMbRtuTransmitComplete, TbxMbRtuDataReceived);
      /* Update the result. */
      result = new_tp_ctx;
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
    tTbxMbTpCtx * tp_ctx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tp_ctx->type == TBX_MB_RTU_CONTEXT_TYPE);
    /* Remove the channel from the lookup table. */
    tbxMbRtuCtx[tp_ctx->port] = NULL;
    /* Invalidate the context to protect it from accidentally being used afterwards. */
    tp_ctx->type = 0U;
    tp_ctx->poll_fcn = NULL;
    tp_ctx->process_fcn = NULL;
    /* Give the transport layer context back to the memory pool. */
    TbxMemPoolRelease(tp_ctx);
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
    tTbxMbTpCtx * tp_ctx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tp_ctx->type == TBX_MB_RTU_CONTEXT_TYPE);
    /* TODO Implement TbxMbRtuPoll(). */
    tp_ctx->poll_fcn = TbxMbRtuPoll; /* Dummy for now. */
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
    tTbxMbTpCtx * tp_ctx = (tTbxMbTpCtx *)event->context;
    /* Make sure the context is valid. */
    TBX_ASSERT(tp_ctx != NULL);
    /* Only continue with a valid context. */
    if (tp_ctx != NULL)
    {
      /* Sanity check on the context type. */
      TBX_ASSERT(tp_ctx->type == TBX_MB_RTU_CONTEXT_TYPE);
      /* TODO Implement TbxMbRtuProcessEvent(). */
      tp_ctx->process_fcn = TbxMbRtuProcessEvent; /* Dummy for now. */
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

  /* TODO Still need to properly handle tp_crx->master_ctx != NULL. In ECL++ Modbus I
   * did a wait loop to make sure INIT state is entered for the master.
   */

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Convert the TP channel pointer to the context structure. */
    tTbxMbTpCtx * tp_ctx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tp_ctx->type == TBX_MB_RTU_CONTEXT_TYPE);
    /* Check that no other packet transmission is already in progress. */
    uint8_t okay_to_transmit = TBX_FALSE;
    TbxCriticalSectionEnter();
    if (tp_ctx->tx_in_progress == TBX_FALSE)
    {
      okay_to_transmit = TBX_TRUE;
      /* Lock access to the tx_packet for the duration of the transmission. Note that
       * the unlock happens in TbxMbRtuTransmitComplete() of it the UART transmission
       * could not be started.
       */
      tp_ctx->tx_in_progress = TBX_TRUE;
    }
    TbxCriticalSectionExit();
    /* Only continue if no other packet transmission is already in progress. */
    if (okay_to_transmit == TBX_TRUE)
    {
      /* Determine ADU specific properties. The ADU starts at one byte before the PDU, 
       * which is the last byte of head[]. The ADU's length is:
       * - Node address (1 byte)
       * - Function code (1 byte)
       * - Packet data (data_len bytes)
       * - CRC16 (2 bytes)
       */
      uint8_t * adu_ptr = &tp_ctx->tx_packet.head[TBX_MB_TP_ADU_HEAD_LEN_MAX-1U];
      uint16_t  adu_len = tp_ctx->tx_packet.data_len + 4U;
      /* Populate the ADU head. For RTU it 
       * is the address field right in front of the
       * PDU. For master->slave transfers the address field is the slave's node address
       * (unicast) or 0 (broadcast). For slave-master transfers it always the slave's
       * node address.
       */
      adu_ptr[0] = tp_ctx->tx_packet.node;
      /* Populate the ADU tail. For RTU it is the CRC16 right after the PDU's data. */
      uint16_t adu_crc = TbxMbRtuCalculatCrc(adu_ptr, adu_len - 2U);
      adu_ptr[adu_len - 2U] = (uint8_t)adu_crc;                         /* CRC16 low.  */
      adu_ptr[adu_len - 1U] = (uint8_t)(adu_crc >> 8U);                 /* CRC16 high. */
      /* Pass ADU transmit request on to the UART module. */
      result = TbxMbUartTransmit(tp_ctx->port, adu_ptr, adu_len);
      /* Unlock access to the tx_packet if the transmission could not be started. */
      if (result != TBX_OK)
      {
        TbxCriticalSectionEnter();
        tp_ctx->tx_in_progress = TBX_FALSE;
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
    tTbxMbTpCtx * tp_ctx = (tTbxMbTpCtx *)transport;
    /* Sanity check on the context type. */
    TBX_ASSERT(tp_ctx->type == TBX_MB_RTU_CONTEXT_TYPE);
    
    /* TODO Implement TbxMbRtuValidate(). */
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
    tTbxMbTpCtx * tp_ctx = tbxMbRtuCtx[port];
    /* Verify transport layer context. */
    TBX_ASSERT(tp_ctx != NULL)
    /* Only continue with a valid transport layer context. Note that there is not need
     * to also check the transport layer type, because only RTU types are stored in the
     * tbxMbRtuCtx[] array.
     */
    if (tp_ctx != NULL)
    {
      /* Unlock access to the tx_packet now that the transmission is complete. */
      TbxCriticalSectionEnter();
      tp_ctx->tx_in_progress = TBX_FALSE;
      TbxCriticalSectionExit();

      /* TODO Set an event for further handling at master/slave module task level. 
       * Probably want to pass the transport layer context to the event.
       * Not yet sure it this module needs its own task function. It might need it for
       * the packet reception state machine.
       * Perhaps it makes more sense to Unlock access to the tx_packet at task level,
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
