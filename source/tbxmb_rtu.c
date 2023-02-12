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
#include "tbxmb_private.h"                       /* MicroTBX-Modbus private            */


/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief RTU specific implementation of the opaque tTbxMbTpChannel pointer. */
typedef struct 
{
  tTbxMbUartPort port;                           /**< UART port linked to the channel. */
} tTbxMbTpRtuChannel;


/* TODO CONTINUE HERE Probaly need to design a uniform transport layer API for PDU
 * transfer/receive. Their function pointers probably need to be stored in a generic
 * non-opaque structure that tTbxMbTpChannel then points to. 
 * 
 * Might also want to add a TP type element to this structure. Can be checks first
 * before casting it from the generic TP opaque pointer to the transport layer specific
 * one, such as RTU.
 * 
 * The transport layer specific structure, such as tTbxMbTpRtuChannel then probably needs
 * to be a copy of that structure (rudimentary derive in a way) and then add its own
 * fields, such as port.
 * 
 * Hmm...but that is error prone though. Maybe just add all the TP specific fields to
 * the generic non-opaque one. Worst case is that it wastes a bit of ROM. For example,
 * a TCP/IP TP would have net-port and ip-address elements, but then also a uart-port
 * element, which it won't need. Not a big deal. Another benefit of this approach is that
 * only one size memory pool is needed to allocate a TP channel context. That actually
 * saves a good chunk of RAM, to definitely go with this approach.
 */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief RTU channel handle lookup table by UART port. */
static tTbxMbTpRtuChannel * tbxMbRtuChannel[TBX_MB_UART_NUM_PORT] = { 0 };


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void TbxMbRtuTransferComplete(tTbxMbUartPort port);
static void TbxMbRtuDataReceived(tTbxMbUartPort port, uint8_t const * data, uint8_t len);


/************************************************************************************//**
** \brief     Creates a Modbus RTU transport layer object.
** \param     port The serial port to use. The actual meaning of the serial port is
**            hardware dependent. It typically maps to the UART peripheral number. E.g. 
**            TBX_MB_UART_PORT1 = USART1 on an STM32.
** \param     baudrate The desired communication speed.
** \param     stopbits Number of stop bits at the end of a character.
** \param     parity Parity bit type to use.
** \return    Handle to the newly created RTU transport layer channel object if
**            successful, NULL otherwise.
**
****************************************************************************************/
tTbxMbTpChannel TbxMbRtuCreate(tTbxMbUartPort port, 
                               tTbxMbUartBaudrate baudrate,
                               tTbxMbUartStopbits stopbits,
                               tTbxMbUartParity parity)
{
  tTbxMbTpChannel result = NULL;

  /* Verify parameters. */
  TBX_ASSERT((port < TBX_MB_UART_NUM_PORT) && 
             (baudrate < TBX_MB_UART_NUM_BAUDRATE) &&
             (stopbits < TBX_MB_UART_NUM_STOPBITS) &&
             (parity < TBX_MB_UART_NUM_PARITY));

  /* Only continue with valid parameters. */
  if ((port < TBX_MB_UART_NUM_PORT) && 
      (baudrate < TBX_MB_UART_NUM_BAUDRATE) &&
      (stopbits < TBX_MB_UART_NUM_STOPBITS) &&
      (parity < TBX_MB_UART_NUM_PARITY))
  {
    /* Allocate memory for the new channel context. */
    tTbxMbTpRtuChannel * new_channel = TbxMemPoolAllocate(sizeof(tTbxMbTpRtuChannel));
    /* Automatically increase the memory pool, if it was too small. */
    if (new_channel == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbTpRtuChannel));
      new_channel = TbxMemPoolAllocate(sizeof(tTbxMbTpRtuChannel));      
    }
    /* Verify memory allocation of the channel context. */
    TBX_ASSERT(new_channel != NULL)
    /* Only continue if the memory allocation succeeded. */
    if (new_channel != NULL)
    {
      /* Initialize the channel. */
      new_channel->port = port;
      /* Store the channel in the lookup table. */
      tbxMbRtuChannel[port] = new_channel;
      /* Initialize the port. Note the RTU always uses 8 databits. */
      TbxMbUartInit(port, baudrate, TBX_MB_UART_8_DATABITS, stopbits, parity,
                    TbxMbRtuTransferComplete, TbxMbRtuDataReceived);
      /* Update the result. */
      result = new_channel;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbRtuCreate ***/  


/************************************************************************************//**
** \brief     Releases a Modbus RTU transport layer object, previously created with 
**            TbxMbRtuCreate().
** \param     channel Handle to RTU transport layer channel object to release.
**
****************************************************************************************/
void TbxMbRtuFree(tTbxMbTpChannel channel)
{
  /* Verify parameters. */
  TBX_ASSERT(channel != NULL);

  /* Only continue with valid parameters. */
  if (channel != NULL)
  {
    /* Convert the TP channel pointer to the RTU one. */
    tTbxMbTpRtuChannel *rtu_channel = (tTbxMbTpRtuChannel *)channel;
    /* Remove the channel from the lookup table. */
    tbxMbRtuChannel[rtu_channel->port] = NULL;
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(rtu_channel);
  }
} /*** end of TbxMbRtuFree ***/


/************************************************************************************//**
** \brief     Event function to signal to this module that the entire transfer completed.
** \attention This function should be called by the UART module.
** \param     port The serial port that the transfer completed on.
**
****************************************************************************************/
static void TbxMbRtuTransferComplete(tTbxMbUartPort port)
{
  /* Verify parameters. */
  TBX_ASSERT(port < TBX_MB_UART_NUM_PORT);

  /* Only continue with valid parameters. */
  if (port < TBX_MB_UART_NUM_PORT)
  {
    /* TODO Implement TbxMbRtuTransferComplete(). Use lookup table with port index to
     * get the channel handle (rtuChannel[]). Probably need to set an event here to
     * further handle this event at task level.
     */
  }
} /*** end of TbxMbRtuTransferComplete ***/


/************************************************************************************//**
** \brief     Event function to signal the reception of new data to this module.
** \attention This function should be called by the UART module.
** \param     port The serial port that the transfer completed on.
** \param     data Byte array with newly received data.
** \param     len Number of newly received bytes.
**
****************************************************************************************/
static void TbxMbRtuDataReceived(tTbxMbUartPort port, uint8_t const * data, uint8_t len)
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
     * the channel handle (rtuChannel[]). Probably need to set an event here to
     * further handle this event at task level.
     */
  }

} /*** end of TbxMbRtuDataReceived ***/


/*********************************** end of tbxmb_uart.c *******************************/
