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
#include "tbxmb_tp_private.h"                    /* MicroTBX-Modbus TP private         */
#include "tbxmb_uart_private.h"                  /* MicroTBX-Modbus UART private       */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief RTU transport layer handle lookup table by UART port. Uses for finding the
 *         transport layer handle that uses a specific serial port, in a run-time
 *         efficient way.
 */
static tTbxMbTransportContext * tbxMbRtuContext[TBX_MB_UART_NUM_PORT] = { 0 };


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
** \return    Handle to the newly created RTU transport layer object if successful, NULL
**            otherwise.
**
****************************************************************************************/
tTbxMbTransport TbxMbRtuCreate(tTbxMbUartPort port, 
                               tTbxMbUartBaudrate baudrate,
                               tTbxMbUartStopbits stopbits,
                               tTbxMbUartParity parity)
{
  tTbxMbTransport result = NULL;

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
    tTbxMbTransportContext * new_transport;
    new_transport = TbxMemPoolAllocate(sizeof(tTbxMbTransportContext));
    /* Automatically increase the memory pool, if it was too small. */
    if (new_transport == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbTransportContext));
      new_transport = TbxMemPoolAllocate(sizeof(tTbxMbTransportContext));      
    }
    /* Verify memory allocation of the transport context. */
    TBX_ASSERT(new_transport != NULL)
    /* Only continue if the memory allocation succeeded. */
    if (new_transport != NULL)
    {
      /* Initialize the transport context. */
      new_transport->type = TBX_MB_TRANSPORT_RTU;
      new_transport->port = port;
      /* Store the transport context in the lookup table. */
      tbxMbRtuContext[port] = new_transport;
      /* Initialize the port. Note the RTU always uses 8 databits. */
      TbxMbUartInit(port, baudrate, TBX_MB_UART_8_DATABITS, stopbits, parity,
                    TbxMbRtuTransferComplete, TbxMbRtuDataReceived);
      /* Update the result. */
      result = new_transport;
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
void TbxMbRtuFree(tTbxMbTransport transport)
{
  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Convert the TP channel pointer to the RTU one. */
    tTbxMbTransportContext *rtu_transport = (tTbxMbTransportContext *)transport;
    /* Sanity check on the transport type. */
    TBX_ASSERT(rtu_transport->type == TBX_MB_TRANSPORT_RTU);
    /* Remove the channel from the lookup table. */
    tbxMbRtuContext[rtu_transport->port] = NULL;
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(rtu_transport);
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
     * get the channel handle (tbxMbRtuContext[]). Probably need to set an event here to
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
     * the channel handle (tbxMbRtuContext[]). Probably need to set an event here to
     * further handle this event at task level.
     */
  }

} /*** end of TbxMbRtuDataReceived ***/


/*********************************** end of tbxmb_uart.c *******************************/
