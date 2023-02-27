/************************************************************************************//**
* \file         tbxmb_event.c
* \brief        Modbus event handler source file.
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
#include "tbxmb_event_private.h"                 /* MicroTBX-Modbus event private      */
#include "tbxmb_osal_private.h"                  /* MicroTBX-Modbus OSAL private       */


/************************************************************************************//**
** \brief     Task function that drives the entire Modbus stack. It processes internally
**            generated events. 
** \details   How to call this function depends on the selected operating system
**            abstraction layer (OSAL) as configured with macro TBX_MB_CONF_OSAL.
**            - In a traditional superloop application (TBX_MB_OPT_OSAL_NONE), call this
**              function continuously in the infinite program loop.
**            - When using an RTOS (e.g. TBX_MB_OPT_OSAL_FREERTOS), create a new task
**              during application initialization and call this function from this task's
**              infinite loop.
**
****************************************************************************************/
void TbxMbEventTask(void)
{
  /* TODO Implement TbxMbEventTask(). */
} /*** end of TbxMbEventTask ***/


/*********************************** end of tbxmb_event.c ******************************/
