/************************************************************************************//**
* \file         tbxmb_server.h
* \brief        Modbus server header file.
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
#ifndef TBXMB_SERVER_H
#define TBXMB_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Handle to a Modbus server channel object, in the format of an opaque
 *         pointer.
 */
typedef void * tTbxMbServer;


/** \brief Enumerated type with all supported return values for the callbacks. */
typedef enum
{
  /* Callback function succeeded. */
  TBX_MB_SERVER_OK = 0U,
  /* Callback function could not perform the request because the address of the specified
   * data element is not supported by the server.
   */
  TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR,
  /* Callback function could not perform the operation on the specified data element. */
  TBX_MB_SERVER_ERR_DEVICE_FAILURE
} tTbxMbServerResult;


/** \brief Modbus server callback function for reading a discrete input. */
typedef tTbxMbServerResult (* tTbxMbServerReadInput)       (tTbxMbServer   channel, 
                                                            uint16_t       addr, 
                                                            uint8_t      * value);


/** \brief Modbus server callback function for reading a coil. */
typedef tTbxMbServerResult (* tTbxMbServerReadCoil)        (tTbxMbServer   channel, 
                                                            uint16_t       addr, 
                                                            uint8_t      * value);


/** \brief Modbus server callback function for writing a coil. */
typedef tTbxMbServerResult (* tTbxMbServerWriteCoil)       (tTbxMbServer   channel, 
                                                            uint16_t       addr, 
                                                            uint8_t        value);


/** \brief Modbus server callback function for reading an input register. */
typedef tTbxMbServerResult (* tTbxMbServerReadInputReg)    (tTbxMbServer   channel, 
                                                            uint16_t       addr, 
                                                            uint16_t     * value);


/** \brief Modbus server callback function for reading a holding register. */
typedef tTbxMbServerResult (* tTbxMbServerReadHoldingReg)  (tTbxMbServer   channel, 
                                                            uint16_t       addr, 
                                                            uint16_t     * value);


/** \brief Modbus server callback function for writing a holding register. */
typedef tTbxMbServerResult (* tTbxMbServerWriteHoldingReg) (tTbxMbServer   channel, 
                                                            uint16_t       addr, 
                                                            uint16_t       value);


/****************************************************************************************
* Function prototypes
****************************************************************************************/
tTbxMbServer TbxMbServerCreate                    (tTbxMbTp                   transport);

void         TbxMbServerFree                      (tTbxMbServer                channel);

void         TbxMbServerSetCallbackReadInput      (tTbxMbServer                channel,
                                                   tTbxMbServerReadInput       callback);

void         TbxMbServerSetCallbackReadCoil       (tTbxMbServer                channel,
                                                   tTbxMbServerReadCoil        callback);

void         TbxMbServerSetCallbackWriteCoil      (tTbxMbServer                channel,
                                                   tTbxMbServerWriteCoil       callback);

void         TbxMbServerSetCallbackReadInputReg   (tTbxMbServer                channel,
                                                   tTbxMbServerReadInputReg    callback);

void         TbxMbServerSetCallbackReadHoldingReg (tTbxMbServer                channel,
                                                   tTbxMbServerReadHoldingReg  callback);

void         TbxMbServerSetCallbackWriteHoldingReg(tTbxMbServer                channel,
                                                   tTbxMbServerWriteHoldingReg callback);


#ifdef __cplusplus
}
#endif

#endif /* TBXMB_SERVER_H */
/*********************************** end of tbxmb_server.h *****************************/
