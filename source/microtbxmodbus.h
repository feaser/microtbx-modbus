/************************************************************************************//**
* \file         microtbxmodbus.h
* \brief        MicroTBX-Modbus header file.
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
#ifndef MICROTBXMODBUS_H
#define MICROTBXMODBUS_H

/****************************************************************************************
* Include files
****************************************************************************************/
#include "tbxmodbusuart.h"                  /* Modbus UART interface                   */


#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************************
* Version definitions
****************************************************************************************/
/** \brief Main version number of MicroTBX-Modbus. */
#define TBX_MODBUS_VERSION_MAIN                  (0U)

/** \brief Minor version number of MicroTBX-Modbus. */
#define TBX_MODBUS_VERSION_MINOR                 (9U)

/** \brief Patch number of MicroTBX-Modbus. */
#define TBX_MODBUS_VERSION_PATCH                 (0U)


#ifdef __cplusplus
}
#endif

#endif /* MICROTBXMODBUS_H */
/*********************************** end of microtbxmodbus.h ***************************/
