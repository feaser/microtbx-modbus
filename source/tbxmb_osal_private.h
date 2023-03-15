/************************************************************************************//**
* \file         tbxmb_osal_private.h
* \brief        Modbus operating system abstration layer private header file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2023 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
*
* SPDX-License-Identifier: GPL-3.0-or-later
*
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
#ifndef TBXMB_OSAL_PRIVATE_H
#define TBXMB_OSAL_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
* Function prototypes
****************************************************************************************/
void    TbxMbOsalEventInit    (void);

void    TbxMbOsalEventPost    (tTbxMbEvent const * event, 
                               uint8_t             fromIsr);

uint8_t TbxMbOsalEventWait    (tTbxMbEvent       * event, 
                               uint16_t            timeoutMs);

void    TbxMbOsalClientInit   (void);

void    TbxMbOsalClientSemGive(uint8_t             fromIsr);

uint8_t TbxMbOsalClientSemTake(uint16_t            timeoutMs);


#ifdef __cplusplus
}
#endif

#endif /* TBXMB_OSAL_PRIVATE_H */
/*********************************** end of tbxmb_osal_private.h **********************/
