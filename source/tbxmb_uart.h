/************************************************************************************//**
* \file         tbxmb_uart.h
* \brief        Modbus UART interface header file.
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
#ifndef TBXMB_UART_H
#define TBXMB_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Enumerated type with all supported UART baudrates. */
typedef enum
{
  /* Communication speed of 1200 bits per second. */
  TBX_MB_UART_1200BPS,
  /* Communication speed of 2400 bits per second. */
  TBX_MB_UART_2400BPS,
  /* Communication speed of 4800 bits per second. */
  TBX_MB_UART_4800BPS,
  /* Communication speed of 9600 bits per second. */
  TBX_MB_UART_9600BPS,
  /* Communication speed of 19200 bits per second. */
  TBX_MB_UART_19200BPS,
  /* Communication speed of 38400 bits per second. */
  TBX_MB_UART_38400BPS,
  /* Communication speed of 57600 bits per second. */
  TBX_MB_UART_57600BPS,
  /* Communication speed of 115200 bits per second. */
  TBX_MB_UART_115200BPS
} tTbxMbUartBaudrate;

/** \brief Enumerated type with all supported UART data bits modes. */
typedef enum
{
  /* 7 data bits mode. */
  TBX_MB_UART_7_DATABITS,
  /* 8 data bits mode. */
  TBX_MB_UART_8_DATABITS
} tTbxMbUartDatabits;

/** \brief Enumerated type with all supported UART stop bits modes. */
typedef enum
{
  /* 1 stop bit mode. */
  TBX_MB_UART_1_STOPBITS,
  /* 2 stop bits mode. */
  TBX_MB_UART_2_STOPBITS
} tTbxMbUartStopbits;

/** \brief Enumerated type with all supported parity modes. */
typedef enum
{
  /* Odd parity mode. */
  TBX_MB_ODD_PARITY,
  /* Even parity mode. */
  TBX_MB_EVEN_PARITY,
  /* No parity mode. */
  TBX_MB_NO_PARITY
} tTbxMbUartParity;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
void TbxMbUartInit(uint8_t serial_port, 
                   tTbxMbUartBaudrate baudrate,
                   tTbxMbUartDatabits databits, 
                   tTbxMbUartStopbits stopbits,
                   tTbxMbUartParity parity);


#ifdef __cplusplus
}
#endif

#endif /* TBXMB_UART_H */
/*********************************** end of tbxmb_uart.h *******************************/
