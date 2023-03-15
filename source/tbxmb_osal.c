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
#include "microtbx.h"                            /* MicroTBX module                    */
#include "microtbxmodbus.h"                      /* MicroTBX-Modbus module             */
#include "tbxmb_checks.h"                        /* MicroTBX-Modbus config checks      */
#include "tbxmb_event_private.h"                 /* MicroTBX-Modbus event private      */
#include "tbxmb_osal_private.h"                  /* MicroTBX-Modbus OSAL private       */
#if (TBX_CONF_OSAL == 1U) /* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#endif


/****************************************************************************************
* Macro definitions
****************************************************************************************/
#ifndef TBX_MB_EVENT_QUEUE_SIZE
/** \brief Configure the size of the event queue. This default configuration should work
 *         just fine in pretty much all usage scenarios. If for some reason a smaller or
 *         larger event queue size is desired, you can override this configuration by
 *         adding a macro with the same name, but a different value, to "tbx_conf.h".
 */
#define TBX_MB_EVENT_QUEUE_SIZE   ((uint8_t)TBX_MB_EVENT_NUM_ID * \
                                   (uint8_t)TBX_MB_UART_NUM_PORT)
#endif


#if (TBX_CONF_OSAL == 0U) /* Superloop */
/****************************************************************************************
*                             O S A L   N O N E
****************************************************************************************/
/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Ring buffer based First-In-First-Out (FIFO) queue for storing events. */
static struct 
{
  tTbxMbEvent entries[TBX_MB_EVENT_QUEUE_SIZE];       /**< Preallocated event storage. */
  uint16_t    count;                                  /**< Number of stored entries.   */
  uint16_t    readIdx;                                /**< Read index into entries[].  */
  uint16_t    writeIdx;                               /**< Write index into entries[]. */
} eventQueue;


/************************************************************************************//**
** \brief     Initialization function for the OSAL module. 
** \attention This function has a built-in protection to make sure it only runs once.
**
****************************************************************************************/
void TbxMbOsalEventInit(void)
{
  static uint8_t osalInitialized = TBX_FALSE;

  /* Only run this function once, */
  if (osalInitialized == TBX_FALSE)
  {
    osalInitialized = TBX_TRUE;
    /* Initialize the queue. */
    eventQueue.count = 0U;
    eventQueue.readIdx = 0U;
    eventQueue.writeIdx = 0U;
  }
} /*** end of TbxMbOsalEventInit ***/


/************************************************************************************//**
** \brief     Signals the occurrence of an event.
** \param     event Pointer to the event to signal.
** \param     fromIsr TBX_TRUE when calling this function from an interrupt service
**            routine, TBX_FALSE otherwise.
**
****************************************************************************************/
void TbxMbOsalEventPost(tTbxMbEvent const * event, 
                        uint8_t             fromIsr)
{
  TBX_UNUSED_ARG(fromIsr);

  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    TbxCriticalSectionEnter();
    /* Make sure there is still space in the queue. If not, then the event queue size is
     * set too small. In this case increase the event queue size using configuration
     * macro TBX_MB_EVENT_QUEUE_SIZE.
     */
    TBX_ASSERT(eventQueue.count < TBX_MB_EVENT_QUEUE_SIZE);

    /* Only continue with enough space. */
    if (eventQueue.count < TBX_MB_EVENT_QUEUE_SIZE)
    {
      /* Store the new event in the queue at the current write index. */
      eventQueue.entries[eventQueue.writeIdx] = *event;
      /* Update the total count. */
      eventQueue.count++;
      /* Increment the write index to point to the next entry. */
      eventQueue.writeIdx++;
      /* Time to wrap around to the start? */
      if (eventQueue.writeIdx == TBX_MB_EVENT_QUEUE_SIZE)
      {
        eventQueue.writeIdx = 0U;
      }
    }
    TbxCriticalSectionExit();
  }
} /*** end of TbxMbOsalEventPost ***/


/************************************************************************************//**
** \brief     Wait for an event to occur.
** \param     event Pointer where the occurred event is written to.
** \param     timeoutMs Maximum time in milliseconds to block while waiting for an
**            event.
** \return    TBX_TRUE if an event occurred, TBX_FALSE otherwise (typically a timeout).
**
****************************************************************************************/
uint8_t TbxMbOsalEventWait(tTbxMbEvent * event,
                           uint16_t      timeoutMs)
{
  uint8_t result = TBX_FALSE;

  TBX_UNUSED_ARG(timeoutMs);

  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    TbxCriticalSectionEnter();
    /* Is there an event available in the queue? */
    if (eventQueue.count > 0U)
    {
      /* Retrieve the event from the queue at the read index (oldest).  */
      *event = eventQueue.entries[eventQueue.readIdx];
      /* Update the total count. */
      eventQueue.count--;
      /* Increment the read index to point to the next entry. */
      eventQueue.readIdx++;
      /* Time to wrap around to the start? */
      if (eventQueue.readIdx == TBX_MB_EVENT_QUEUE_SIZE)
      {
        eventQueue.readIdx = 0U;
      }
      /* Update the result. */
      result = TBX_TRUE;
    }
    TbxCriticalSectionExit();
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbOsalEventWait ***/


/************************************************************************************//**
** \brief     Initialization function for the OSAL client module. 
** \attention This function has a built-in protection to make sure it only runs once.
**
****************************************************************************************/
void TbxMbOsalClientInit(void)
{
  static uint8_t osalClientInitialized = TBX_FALSE;

  /* Only run this function once, */
  if (osalClientInitialized == TBX_FALSE)
  {
    osalClientInitialized = TBX_TRUE;
    /* TODO Implement TbxMbOsalClientInit(). */
  }
} /*** end of TbxMbOsalClientInit ***/


/************************************************************************************//**
** \brief     Give the semaphore.
** \param     fromIsr TBX_TRUE when calling this function from an interrupt service
**            routine, TBX_FALSE otherwise.
**
****************************************************************************************/
void TbxMbOsalClientSemGive(uint8_t fromIsr)
{
  TBX_UNUSED_ARG(fromIsr);

  /* TODO Implement TbxMbOsalClientSemGive(). */
} /*** end of TbxMbOsalClientSemGive ***/


/************************************************************************************//**
** \brief     Take the semaphore when available or wait a finite amount of time for it to
**            become available.
** \param     timeoutMs Maximum time in milliseconds to block while waiting for the 
**            semaphore to become available.
** \return    TBX_TRUE if the semaphore could be takem, TBX_FALSE otherwise (typically a
**            timeout).
**
****************************************************************************************/
uint8_t TbxMbOsalClientSemTake(uint16_t timeoutMs)
{
  uint8_t result = TBX_FALSE;

  TBX_UNUSED_ARG(timeoutMs);

  /* TODO Implement TbxMbOsalClientSemTake(). */

  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbOsalClientSemTake ***/
#endif /* (TBX_CONF_OSAL == 0U) */


#if (TBX_CONF_OSAL == 1U) /* FreeRTOS */
/****************************************************************************************
*                             O S A L   F R E E R T O S
****************************************************************************************/
/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Queue handle for storing events. */
static QueueHandle_t     eventQueue;

/** \brief Semaphore handle for the client. */
static SemaphoreHandle_t clientSem;


/************************************************************************************//**
** \brief     Initialization function for the OSAL event module. 
** \attention This function has a built-in protection to make sure it only runs once.
**
****************************************************************************************/
void TbxMbOsalEventInit(void)
{
  static uint8_t osalInitialized = TBX_FALSE;

  /* Only run this function once, */
  if (osalInitialized == TBX_FALSE)
  {
    osalInitialized = TBX_TRUE;
    /* Create the event queue. */
    eventQueue = xQueueCreate(TBX_MB_EVENT_QUEUE_SIZE, sizeof(tTbxMbEvent));
    /* Check that the queue creation was successful. If this assertion fails, increase
     * the FreeRTOS heap size.
     */
    TBX_ASSERT(eventQueue != NULL);
  }
} /*** end of TbxMbOsalEventInit ***/


/************************************************************************************//**
** \brief     Signals the occurrence of an event.
** \param     event Pointer to the event to signal.
** \param     fromIsr TBX_TRUE when calling this function from an interrupt service
**            routine, TBX_FALSE otherwise.
**
****************************************************************************************/
void TbxMbOsalEventPost(tTbxMbEvent const * event, 
                        uint8_t             fromIsr)
{
  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* Not calling from an ISR? */
    if (fromIsr == TBX_FALSE)
    {
      /* Add the event to the queue. There should be space in the queue so no need to
       * wait for a spot to become available in the queue.
       */
      BaseType_t queueResult = xQueueSend(eventQueue, (void const *)event, 0U);
      /* Make sure the event could be added. If not, then the event queue size is set
       * too small. In this case increase the event queue size using configuration
       * macro TBX_MB_EVENT_QUEUE_SIZE.
       */
      TBX_ASSERT(queueResult == pdTRUE);
    }
    /* Calling from an ISR. */
    else
    {
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;

      /* Add the event to the queue. There should be space in the queue, so this should
       * always succeed.
       */
      BaseType_t queueResult = xQueueSendFromISR(eventQueue, event, 
                                                 &xHigherPriorityTaskWoken);
      /* Make sure the event could be added. If not, then the event queue size is set
       * too small. In this case increase the event queue size using configuration
       * macro TBX_MB_EVENT_QUEUE_SIZE.
       */
      TBX_ASSERT(queueResult == pdTRUE);
      /* Request scheduler to switch to the higher priority task, if is was woken. Note
       * that this part is FreeRTOS port specific. The following works on all Cortex-M
       * ports. Might need to add conditional compilation switches to support other
       * ports in the future.
       */
      if (xHigherPriorityTaskWoken != pdFALSE)
      {
        portYIELD();
      }
    }
  }
} /*** end of TbxMbOsalEventPost ***/


/************************************************************************************//**
** \brief     Wait for an event to occur.
** \param     event Pointer where the occurred event is written to.
** \param     timeoutMs Maximum time in milliseconds to block while waiting for an
**            event.
** \return    TBX_TRUE if an event occurred, TBX_FALSE otherwise (typically a timeout).
**
****************************************************************************************/
uint8_t TbxMbOsalEventWait(tTbxMbEvent * event,
                           uint16_t      timeoutMs)
{
  uint8_t result = TBX_FALSE;

  /* Verify parameters. */
  TBX_ASSERT(event != NULL);

  /* Only continue with valid parameters. */
  if (event != NULL)
  {
    /* Wait for a new event to arrive in the queue. */
    if (xQueueReceive(eventQueue, event, pdMS_TO_TICKS(timeoutMs)) == pdTRUE)
    {
      result = TBX_TRUE;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbOsalEventWait ***/


/************************************************************************************//**
** \brief     Initialization function for the OSAL client module. 
** \attention This function has a built-in protection to make sure it only runs once.
**
****************************************************************************************/
void TbxMbOsalClientInit(void)
{
  static uint8_t osalClientInitialized = TBX_FALSE;

  /* Only run this function once, */
  if (osalClientInitialized == TBX_FALSE)
  {
    osalClientInitialized = TBX_TRUE;
    /* Create the client binary semaphore, which is initially taken (count = 0). */
    clientSem = xSemaphoreCreateBinary();
    /* Check that the semaphore creation was successful. If this assertion fails,
     * increase the FreeRTOS heap size.
     */
    TBX_ASSERT(clientSem != NULL);
  }
} /*** end of TbxMbOsalClientInit ***/


/************************************************************************************//**
** \brief     Give the semaphore.
** \param     fromIsr TBX_TRUE when calling this function from an interrupt service
**            routine, TBX_FALSE otherwise.
**
****************************************************************************************/
void TbxMbOsalClientSemGive(uint8_t fromIsr)
{
  /* Not calling from an ISR? */
  if (fromIsr == TBX_FALSE)
  {
    /* Give the semaphore, which increments its count. The return value can be ignored.
     * If only fails if the semaphore was already in the given state, which is fine
     * because then this function still achieved what it was meant to do.
     */
    (void)xSemaphoreGive(clientSem);
  }
  /* Calling from an ISR. */
  else
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Give the semaphore, which increments its count. The return value can be ignored.
     * If only fails if the semaphore was already in the given state, which is fine
     * because then this function still achieved what it was meant to do.
     */
    (void)xSemaphoreGiveFromISR(clientSem, &xHigherPriorityTaskWoken);
    /* Request scheduler to switch to the higher priority task, if is was woken. Note
     * that this part is FreeRTOS port specific. The following works on all Cortex-M
     * ports. Might need to add conditional compilation switches to support other
     * ports in the future.
     */
    if (xHigherPriorityTaskWoken != pdFALSE)
    {
      portYIELD();
    }
  }
} /*** end of TbxMbOsalClientSemGive ***/


/************************************************************************************//**
** \brief     Take the semaphore when available or wait a finite amount of time for it to
**            become available.
** \param     timeoutMs Maximum time in milliseconds to block while waiting for the 
**            semaphore to become available.
** \return    TBX_TRUE if the semaphore could be takem, TBX_FALSE otherwise (typically a
**            timeout).
**
****************************************************************************************/
uint8_t TbxMbOsalClientSemTake(uint16_t timeoutMs)
{
  uint8_t result = TBX_FALSE;

  /* Wait for the semaphore to become available. */
  if (xSemaphoreTake(clientSem, pdMS_TO_TICKS(timeoutMs)) == pdTRUE)
  {
    result = TBX_TRUE;
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbOsalClientSemTake ***/
#endif /* (TBX_CONF_OSAL == 1U) */


/*********************************** end of tbxmb_osal.c *******************************/
