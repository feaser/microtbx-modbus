/************************************************************************************//**
* \file         tbxmodbusuart.h
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
#ifndef TBXMODBUSUART_H
#define TBXMODBUSUART_H

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
  TBX_MODBUS_UART_1200BPS,
  /* Communication speed of 2400 bits per second. */
  TBX_MODBUS_UART_2400BPS,
  /* Communication speed of 4800 bits per second. */
  TBX_MODBUS_UART_4800BPS,
  /* Communication speed of 9600 bits per second. */
  TBX_MODBUS_UART_9600BPS,
  /* Communication speed of 19200 bits per second. */
  TBX_MODBUS_UART_19200BPS,
  /* Communication speed of 38400 bits per second. */
  TBX_MODBUS_UART_38400BPS,
  /* Communication speed of 57600 bits per second. */
  TBX_MODBUS_UART_57600BPS,
  /* Communication speed of 115200 bits per second. */
  TBX_MODBUS_UART_115200BPS
} tTbxModbusUartBaudrate;

/** \brief Enumerated type with all supported UART data bits modes. */
typedef enum
{
  /* 7 data bits mode. */
  TBX_MODBUS_UART_7_DATABITS,
  /* 8 data bits mode. */
  TBX_MODBUS_UART_8_DATABITS
} tTbxModbusUartDatabits;

/** \brief Enumerated type with all supported UART stop bits modes. */
typedef enum
{
  /* 1 stop bit mode. */
  TBX_MODBUS_UART_1_STOPBITS,
  /* 2 stop bits mode. */
  TBX_MODBUS_UART_2_STOPBITS
} tTbxModbusUartStopbits;

/** \brief Enumerated type with all supported parity modes. */
typedef enum
{
  /* Odd parity mode. */
  TBX_MODBUS_ODD_PARITY,
  /* Even parity mode. */
  TBX_MODBUS_EVEN_PARITY,
  /* No parity mode. */
  TBX_MODBUS_NO_PARITY
} tTbxModbusUartParity;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
void TbxModbusUartInit(uint8_t serial_port, 
                       tTbxModbusUartBaudrate baudrate,
                       tTbxModbusUartDatabits databits, 
                       tTbxModbusUartStopbits stopbits,
                       tTbxModbusUartParity parity);


#ifdef __cplusplus
}
#endif

#endif /* TBXMODBUSUART_H */
/*********************************** end of tbxmodbusuart.h ****************************/
