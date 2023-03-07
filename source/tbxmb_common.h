/************************************************************************************//**
* \file         tbxmb_common.h
* \brief        MicroTBX-Modbus common definitions header file.
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
#ifndef TBXMB_COMMON_H
#define TBXMB_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Modbus function code 04 - Read Input Register. */
#define TBX_MB_FC04_READ_INPUT_REGISTER               (4U)

/** \brief Modbus exception code 01 - Illegal function. */
#define TBX_MB_EC01_ILLEGAL_FUNCTION                  (1U)

/** \brief Modbus exception code 02 - Illegal data address. */
#define TBX_MB_EC02_ILLEGAL_DATA_ADDRESS              (2U)

/** \brief Modbus exception code 03 - Illegal data value. */
#define TBX_MB_EC03_ILLEGAL_DATA_VALUE                (3U)

/** \brief Modbus exception code 04 - Server device failure. */
#define TBX_MB_EC04_SERVER_DEVICE_FAILURE             (4U)

/** \brief Bit mask to OR to the function code to flag it as an exception response. */
#define TBX_MB_FC_EXCEPTION_MASK                      (0x80U)


#ifdef __cplusplus
}
#endif

#endif /* TBXMB_COMMON_H */
/*********************************** end of tbxmb_common.h *****************************/
