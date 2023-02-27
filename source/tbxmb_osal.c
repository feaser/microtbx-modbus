/************************************************************************************//**
* \file         tbxmb_osal.c
* \brief        Modbus operating system abstration layer source file.
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


/****************************************************************************************
* Local data declarations
****************************************************************************************/
static uint8_t osalInitialized = TBX_FALSE;


/************************************************************************************//**
** \brief     Initialization function for the OSAL module. 
** \attention This function has a built-in protection to make sure it only runs once.
**
****************************************************************************************/
void TbxMbOsalInit(void)
{
  /* Only run this function once, */
  if (osalInitialized == TBX_FALSE)
  {
    osalInitialized = TBX_TRUE;

    /* TODO Implement TbxMbOsalInit(). */
  }
} /*** end of TbxMbOsalInit ***/


/*********************************** end of tbxmb_osal.c *******************************/
