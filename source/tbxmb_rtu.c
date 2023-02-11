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
tTbxMbRtuHandle TbxMbRtuCreate(tTbxMbUartPort port, 
                               tTbxMbUartBaudrate baudrate,
                               tTbxMbUartStopbits stopbits,
                               tTbxMbUartParity parity)
{
  tTbxMbRtuHandle result = NULL;

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
    /* TODO Implement TbxMbRtuCreate(). */
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbRtuCreate ***/  


/*********************************** end of tbxmb_uart.c *******************************/
