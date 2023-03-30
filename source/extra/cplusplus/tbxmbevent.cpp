/************************************************************************************//**
* \file         tbxmbevent.cpp
* \brief        MicroTBX-Modbus event handling C++ source file.
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

/****************************************************************************************
* Include files
****************************************************************************************/
#include "microtbx.h"                            /* MicroTBX library                   */
#include "microtbxmodbus.hpp"                    /* MicroTBX-Modbus C++ library        */


/****************************************************************************************
*                            T B X M B E V E N T
****************************************************************************************/
/************************************************************************************//**
** \brief     Task method that drives the entire Modbus stack. It processes internally
**            generated events. 
** \details   How to call this method depends on the selected operating system
**            abstraction layer (OSAL):
**            - In a traditional superloop application (tbxmb_superloop.c), call this
**              method continuously in the infinite program loop.
**            - When using an RTOS (e.g. tbxmb_freertos.c), create a new task during
**              application initialization and call this method from this task's 
**              infinite loop.
**
**            When using a traditional superloop application in combination with just
**            a Modbus client, you can omit the call to this method. With this
**            combination, the communication with a Modbus server happens in a blocking
**            manner and the event task is automatically called internally, while
**            blocking. Convenient and easy, but not optimal from a run-time performance.
**            For this reason it is recommended to use an RTOS in combination with a
**            Modbus client.

**
****************************************************************************************/
void TbxMbEvent::task()
{
  TbxMbEventTask();
} /*** end of task ***/

/*********************************** end of tbxmbevent.cpp ******************************/
