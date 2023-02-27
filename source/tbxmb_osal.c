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


#if (TBX_MB_CONF_OSAL == TBX_MB_OPT_OSAL_NONE)
/****************************************************************************************
*                             O S A L   N O N E
****************************************************************************************/
/************************************************************************************//**
** \brief     Initialization function for the OSAL module. 
** \attention This function has a built-in protection to make sure it only runs once.
**
****************************************************************************************/
void TbxMbOsalInit(void)
{
  static uint8_t osalInitialized = TBX_FALSE;

  /* Only run this function once, */
  if (osalInitialized == TBX_FALSE)
  {
    osalInitialized = TBX_TRUE;

    /* TODO Implement TbxMbOsalInit(). */
  }
} /*** end of TbxMbOsalInit ***/


/************************************************************************************//**
** \brief     Signals the occurrence of an event.
** \param     event Pointer to the event to signal.
** \param     fromIsr TBX_TRUE when calling this function from an interrupt service
**            routine, TBX_FALSE otherwise.
**
****************************************************************************************/
void TbxMbOsalPostEvent(tTbxMbEvent * event, 
                        uint8_t       from_isr)
{
  TBX_UNUSED_ARG(from_isr);

  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* TODO Implement TbxMbOsalPostEvent(). */
    event->context = NULL; /* Dummy for now. */
  }
} /*** end of TbxMbOsalPostEvent ***/


/************************************************************************************//**
** \brief     Wait for an event to occur.
** \param     event Pointer where the occurred event is written to.
** \param     timeout_ms Maximum time in milliseconds to block while waiting for an
**            event.
** \return    TBX_TRUE if an event occurred, TBX_FALSE otherwise (typically a timeout).
**
****************************************************************************************/
uint8_t TbxMbOsalWaitEvent(tTbxMbEvent * event,
                           uint16_t      timeout_ms)
{
  uint8_t result = TBX_FALSE;

  TBX_UNUSED_ARG(timeout_ms);

  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* TODO Implement TbxMbOsalWaitEvent(). */
    event->context = NULL; /* Dummy for now. */
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbOsalWaitEvent ***/
#endif /* (TBX_MB_CONF_OSAL == TBX_MB_OPT_OSAL_NONE) */


#if (TBX_MB_CONF_OSAL == TBX_MB_OPT_OSAL_FREERTOS)
/****************************************************************************************
*                             O S A L   F R E E R T O S
****************************************************************************************/
/************************************************************************************//**
** \brief     Initialization function for the OSAL module. 
** \attention This function has a built-in protection to make sure it only runs once.
**
****************************************************************************************/
void TbxMbOsalInit(void)
{
  static uint8_t osalInitialized = TBX_FALSE;

  /* Only run this function once, */
  if (osalInitialized == TBX_FALSE)
  {
    osalInitialized = TBX_TRUE;

    /* TODO Implement TbxMbOsalInit(). */
  }
} /*** end of TbxMbOsalInit ***/


/************************************************************************************//**
** \brief     Signals the occurrence of an event.
** \param     event Pointer to the event to signal.
** \param     fromIsr TBX_TRUE when calling this function from an interrupt service
**            routine, TBX_FALSE otherwise.
**
****************************************************************************************/
void TbxMbOsalPostEvent(tTbxMbEvent * event, 
                        uint8_t       from_isr)
{
  TBX_UNUSED_ARG(from_isr);

  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* TODO Implement TbxMbOsalPostEvent(). */
    event->context = NULL; /* Dummy for now. */
  }
} /*** end of TbxMbOsalPostEvent ***/


/************************************************************************************//**
** \brief     Wait for an event to occur.
** \param     event Pointer where the occurred event is written to.
** \param     timeout_ms Maximum time in milliseconds to block while waiting for an
**            event.
** \return    TBX_TRUE if an event occurred, TBX_FALSE otherwise (typically a timeout).
**
****************************************************************************************/
uint8_t TbxMbOsalWaitEvent(tTbxMbEvent * event,
                           uint16_t      timeout_ms)
{
  uint8_t result = TBX_FALSE;

  TBX_UNUSED_ARG(timeout_ms);

  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* TODO Implement TbxMbOsalWaitEvent(). */
    event->context = NULL; /* Dummy for now. */
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbOsalWaitEvent ***/
#endif /* (TBX_MB_CONF_OSAL == TBX_MB_OPT_OSAL_FREERTOS) */


/*********************************** end of tbxmb_osal.c *******************************/
