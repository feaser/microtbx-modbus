/************************************************************************************//**
* \file         tbxmb_tp_private.h
* \brief        Modbus transport layer private header file.
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
#ifndef TBXMB_TP_PRIVATE_H
#define TBXMB_TP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Enumerated type with all supported transport layer types. */
typedef enum
{
  /* RTU. */
  TBX_MB_TRANSPORT_RTU = 0U,
  /* ASCII. */
  TBX_MB_TRANSPORT_ASCII,
  /* Extra entry to obtain the number of elements. */
  TBX_MB_TRANSPORT_NUM_TYPES
} tTbxMbTransportType;

/** \brief   Modbus transport layer context that groups all transport layer specific
 *           data. It's what the tTbxMbTransport opaque pointer points to.
 *  \details For both simplicity and run-time efficiency, this type packs information for
 *           all different transport layers, even though some elements are not needed
 *           for a specific transport layer. For example, a TCP/IP transport layer does
 *           not really need the serial port field that the RTU/ASCII transport layers
 *           need.
 *           These context are allocated using a memory pool. By having one generic 
 *           transport layer type, only one size memory pool size is needed. If a type
 *           would be created for each specific transport layer, these types might have
 *           different sizes and would require multiple memory pools of different sizes.
 *           So at the end it is actually more RAM efficient to group the elements of
 *           all transport layers in one generic one.
 */
typedef struct 
{
  tTbxMbTransportType type;                      /**< Transport layer type.            */
  tTbxMbUartPort port;                           /**< UART port linked to the channel. */
} tTbxMbTransportContext;

/* TODO CONTINUE HERE Probaly need to design a uniform transport layer API for PDU
 * transfer/receive. Their function pointers probably need to be stored in the 
 * tTbxMbTransportContext.
 * 
 * Probably want to add ADU/PDU data structures inside tTbxMbTransportContext as well.
 * That would make it possible to have copy free storage of the data packets. At least
 * for transmit. Might need to add a busy flag for MUX locking purposes.
 */


#ifdef __cplusplus
}
#endif

#endif /* TBXMB_TP_PRIVATE_H */
/*********************************** end of tbxmb_tp_private.h *************************/
