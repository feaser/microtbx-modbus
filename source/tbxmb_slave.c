/************************************************************************************//**
* \file         tbxmb_slave.c
* \brief        Modbus slave source file.
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
#include "tbxmb_tp_private.h"                    /* MicroTBX-Modbus TP private         */
#include "tbxmb_osal_private.h"                  /* MicroTBX-Modbus OSAL private       */
#include "tbxmb_slave_private.h"                 /* MicroTBX-Modbus slave private      */


/************************************************************************************//**
** \brief     Creates a Modbus slave channel object and assigs the specified Modbus
**            transport layer to the channel for packet transmission and reception.
** \param     transport Handle to a previously created Modbus transport layer object to
**            assign to the channel.
** \return    Handle to the newly created Modbus slave channel object if successful, NULL
**            otherwise.
**
****************************************************************************************/
tTbxMbSlave TbxMbSlaveCreate(tTbxMbTp transport)
{
  tTbxMbSlave result = NULL;

  /* Make sure the OSAL module is initialized. */
  TbxMbOsalInit();

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Allocate memory for the new channel context. */
    tTbxMbSlaveCtx * new_slave_ctx = TbxMemPoolAllocate(sizeof(tTbxMbSlaveCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (new_slave_ctx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbSlaveCtx));
      new_slave_ctx = TbxMemPoolAllocate(sizeof(tTbxMbSlaveCtx));      
    }
    /* Verify memory allocation of the channel context. */
    TBX_ASSERT(new_slave_ctx != NULL)
    /* Only continue if the memory allocation succeeded. */
    if (new_slave_ctx != NULL)
    {
      /* Convert the TP channel pointer to the context structure. */
      tTbxMbTpCtx * tp_ctx = (tTbxMbTpCtx *)transport;
      /* Initialize the channel context. Start by crosslinking the transport layer. */
      new_slave_ctx->tp_ctx = tp_ctx;
      new_slave_ctx->tp_ctx->master_ctx = NULL;
      new_slave_ctx->tp_ctx->slave_ctx = new_slave_ctx;
      /* Update the result. */
      result = new_slave_ctx;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbSlaveCreate ****/


/************************************************************************************//**
** \brief     Releases a Modbus slave channel object, previously created with
**            TbxMbSlaveCreate().
** \param     channel Handle to the Modbus slave channel object to release.
**
****************************************************************************************/
void TbxMbSlaveFree(tTbxMbSlave channel)
{
  /* Verify parameters. */
  TBX_ASSERT(channel != NULL);

  /* Only continue with valid parameters. */
  if (channel != NULL)
  {
    /* Convert the slave channel pointer to the context structure. */
    tTbxMbSlaveCtx * slave_ctx = (tTbxMbSlaveCtx *)channel;
    /* Remove crosslink between the channel and the transport layer. */
    TbxCriticalSectionEnter();
    slave_ctx->tp_ctx->slave_ctx = NULL;
    slave_ctx->tp_ctx = NULL;
    TbxCriticalSectionExit();
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(slave_ctx);
  }
} /*** end of TbxMbSlaveFree ***/


/*********************************** end of tbxmb_slave.c ******************************/
