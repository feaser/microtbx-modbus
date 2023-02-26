/************************************************************************************//**
* \file         tbxmb_master.c
* \brief        Modbus master source file.
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
#include "tbxmb_master_private.h"                /* MicroTBX-Modbus master private     */


/************************************************************************************//**
** \brief     Creates a Modbus master channel object and assigns the specified Modbus
**            transport layer to the channel for packet transmission and reception.
** \param     transport Handle to a previously created Modbus transport layer object to
**            assign to the channel.
** \return    Handle to the newly created Modbus master channel object if successful,
**            NULL otherwise.
**
****************************************************************************************/
tTbxMbMaster TbxMbMasterCreate(tTbxMbTp transport)
{
  tTbxMbMaster result = NULL;

  /* Verify parameters. */
  TBX_ASSERT(transport != NULL);

  /* Only continue with valid parameters. */
  if (transport != NULL)
  {
    /* Allocate memory for the new channel context. */
    tTbxMbMasterCtx * new_master_ctx = TbxMemPoolAllocate(sizeof(tTbxMbMasterCtx));
    /* Automatically increase the memory pool, if it was too small. */
    if (new_master_ctx == NULL)
    {
      /* No need to check the return value, because if it failed, the following
       * allocation fails too, which is verified later on.
       */
      (void)TbxMemPoolCreate(1U, sizeof(tTbxMbMasterCtx));
      new_master_ctx = TbxMemPoolAllocate(sizeof(tTbxMbMasterCtx));      
    }
    /* Verify memory allocation of the channel context. */
    TBX_ASSERT(new_master_ctx != NULL)
    /* Only continue if the memory allocation succeeded. */
    if (new_master_ctx != NULL)
    {
      /* Convert the TP channel pointer to the context structure. */
      tTbxMbTpCtx * tp_ctx = (tTbxMbTpCtx *)transport;
      /* Initialize the channel context. Start by crosslinking the transport layer. */
      new_master_ctx->tp_ctx = tp_ctx;
      new_master_ctx->tp_ctx->master_ctx = new_master_ctx;
      new_master_ctx->tp_ctx->slave_ctx = NULL;
      /* Update the result. */
      result = new_master_ctx;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of TbxMbMasterCreate ****/


/************************************************************************************//**
** \brief     Releases a Modbus master channel object, previously created with
**            TbxMbSlaveCreate().
** \param     channel Handle to the Modbus master channel object to release.
**
****************************************************************************************/
void TbxMbMasterFree(tTbxMbMaster channel)
{
  /* Verify parameters. */
  TBX_ASSERT(channel != NULL);

  /* Only continue with valid parameters. */
  if (channel != NULL)
  {
    /* Convert the master channel pointer to the context structure. */
    tTbxMbMasterCtx * master_ctx = (tTbxMbMasterCtx *)channel;
    /* Remove crosslink between the channel and the transport layer. */
    TbxCriticalSectionEnter();
    master_ctx->tp_ctx->master_ctx = NULL;
    master_ctx->tp_ctx = NULL;
    TbxCriticalSectionExit();
    /* Give the channel context back to the memory pool. */
    TbxMemPoolRelease(master_ctx);
  }
} /*** end of TbxMbMasterFree ***/


/*********************************** end of tbxmb_master.c *****************************/
