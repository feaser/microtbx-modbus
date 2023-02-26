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
* Macro definitions
****************************************************************************************/
/** \brief Maximum size of the "Function code" at the start of a PDU. */
#define TBX_MB_TP_PDU_CODE_LEN_MAX     (1U)

/** \brief Maximum number of data bytes inside a PDU. This excludes the function code. */
#define TBX_MB_TP_PDU_DATA_LEN_MAX     (252U)

/** \brief Maximum length of an "Protocol Data Unit". */
#define TBX_MB_TP_PDU_MAX_LEN          (TBX_MB_TP_PDU_CODE_LEN_MAX + \
                                        TBX_MB_TP_PDU_DATA_LEN_MAX) 

/** \brief Maximum ADU overhead bytes before the actual PDU, Called "Additional address"
 *         in the Modbus protocol.
 */
#define TBX_MB_TP_ADU_HEAD_LEN_MAX     (7U)

/** \brief Maximum ADU overhead bytes after the actual PDU. Caled "Error check" in the 
 *         Modbus protocol.
 */
#define TBX_MB_TP_ADU_TAIL_LEN_MAX     (2U)

/** \brief Maximum length of an "Application Data Unit" which packs communication layer
 *         data around a PDU.
 */
#define TBX_MB_TP_ADU_MAX_LEN          (TBX_MB_TP_ADU_HEAD_LEN_MAX + \
                                        TBX_MB_TP_PDU_MAX_LEN + \
                                        TBX_MB_TP_ADU_TAIL_LEN_MAX)


/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Enumerated type with all supported transport layer types. Can be used after
 *         casting the opaque tTbxMbTransport pointer to a tTbxMbTransportContext
 *         pointer, to check if it's a handle for the correct transport layer.
 */
typedef enum
{
  /* RTU. */
  TBX_MB_TP_RTU = 0U,
  /* ASCII. */
  TBX_MB_TP_ASCII,
  /* Extra entry to obtain the number of elements. */
  TBX_MB_TP_NUM_TYPES
} tTbxMbTpType;


/** \brief Type for grouping all "Protocol Data Unit" data together. */
typedef struct
{
  uint8_t code;                                        /**< PDU function code.         */
  uint8_t data[TBX_MB_TP_PDU_DATA_LEN_MAX];            /**< PDU data bytes.            */
} tTbxMbTpPdu;


/** \brief Type for grouping all communication packet related data together. */
typedef struct 
{
  uint8_t     head[TBX_MB_TP_ADU_HEAD_LEN_MAX];        /**< ADU additional address.    */
  tTbxMbTpPdu pdu;                                     /**< Protocol data unit.        */
  uint8_t     tail[TBX_MB_TP_ADU_TAIL_LEN_MAX];        /**< ADU error check.           */
  uint8_t     data_len;                                /**< Number of PDU data bytes.  */
  uint8_t     node;                                    /**< Node identifier.           */
} tTbxMbTpPacket;


/** \brief Transport layer interface function to start the transmission of the data 
 *         packet, stored in the transport layer context.
 */
typedef uint8_t (* tTbxMbTpTransmit)(tTbxMbTp transport);


/** \brief Transport layer interface function to validate a newly received packet, stored
 *         in the transport layer context. This is transport layer specific. For example
 *         on RTU this means that the CRC16 in the tail ofthe ADU needs to be verified.
 */
typedef uint8_t (* tTbxMbTpValidate)(tTbxMbTp transport);


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
  tTbxMbTpType     type;                         /**< Transport layer type.            */
  uint8_t          node_addr;                    /**< Node address (RTU/ASCII only).   */
  tTbxMbUartPort   port;                         /**< UART port (RTU/ASCII only)     . */
  tTbxMbTpPacket   tx_packet;                    /**< Transmit packet buffer.          */
  uint8_t          tx_in_progress;               /**< Transmit packet MUX flag.        */
  tTbxMbTpPacket   rx_packet;                    /**< Reception packet buffer.         */
  uint8_t          rx_in_progress;               /**< Reception packet MUX flag.       */
  tTbxMbTpTransmit transmit_fcn;                 /**< Packet transmit function.        */
  tTbxMbTpValidate validate_fcn;                 /**< Rx Packet validate function.     */
  uint8_t          master;                       /**< Master or slave boolean flag.    */
} tTbxMbTpContext;


#ifdef __cplusplus
}
#endif

#endif /* TBXMB_TP_PRIVATE_H */
/*********************************** end of tbxmb_tp_private.h *************************/
