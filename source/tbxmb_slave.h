/************************************************************************************//**
* \file         tbxmb_slave.h
* \brief        Modbus slave header file.
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
#ifndef TBXMB_SLAVE_H
#define TBXMB_SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief Handle to a Modbus slave channel object, in the format of an opaque
 *         pointer.
 */
typedef void * tTbxMbSlave;


/****************************************************************************************
* Function prototypes
****************************************************************************************/
tTbxMbSlave TbxMbSlaveCreate(tTbxMbTp transport);
void        TbxMbSlaveFree(tTbxMbSlave channel);
/* TODO Add API for registering the Read/Write coil/input/registers callbacks. Store
 * the function pointers in tTbxMbSlaveCtx.
 */

#ifdef __cplusplus
}
#endif

#endif /* TBXMB_SLAVE_H */
/*********************************** end of tbxmb_slave.h ******************************/
