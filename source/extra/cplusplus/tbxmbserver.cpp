/************************************************************************************//**
* \file         tbxmbserver.cpp
* \brief        MicroTBX-Modbus server C++ source file.
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
*                            T B X M B S E R V E R
****************************************************************************************/
/************************************************************************************//**
** \brief     Modbus server base destructor.
**
****************************************************************************************/
TbxMbServer::~TbxMbServer()
{
  /* Nothing to do here but do not remove this destructor. The destructor was made pure
   * virtual to force the class to be abstract. This was needed because it doesn't
   * contain any pure virtual methods. 
   * Note that the destructor is the only method that, even if it is pure virtual, has to 
   * have an implementation. Otherwise derived classes cannot be instantiated.
   */
} /*** end of ~TbxMbServer ***/


/************************************************************************************//**
** \brief     Reads a data element from the discrete input registers data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \param     addr Element address (0..65535).
** \param     value Reference where to store the value of the discrete input.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::readInput(uint16_t addr, 
                                          bool&    value)
{
  return TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
} /*** end of readInput ***/


/************************************************************************************//**
** \brief     Reads a data element from the coils data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \param     addr Element address (0..65535).
** \param     value Reference where to store the value of the coil.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::readCoil(uint16_t addr,
                                         bool&    value)
{
  return TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
} /*** end of readCoil ***/


/************************************************************************************//**
** \brief     Writes a data element to the coils data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \param     addr Element address (0..65535).
** \param     value Coil value.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::writeCoil(uint16_t addr, 
                                          bool    value)
{
  return TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
} /*** end of writeCoil ***/


/************************************************************************************//**
** \brief     Reads a data element from the input registers data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \attention Store the value of the input register in your CPUs native endianess. The
**            MicroTBX-Modbus stack will automatically convert this to the big endianess
**            that the Modbus protocol requires.
** \param     addr Element address (0..65535).
** \param     value Reference where to store the value of the input register.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::readInputReg(uint16_t  addr, 
                                             uint16_t& value)
{
  return TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
} /*** end of readInputReg ***/


/************************************************************************************//**
** \brief     Reads a data element from the holding registers data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \attention Store the value of the holding register in your CPUs native endianess. The
**            MicroTBX-Modbus stack will automatically convert this to the big endianess
**            that the Modbus protocol requires.
** \param     addr Element address (0..65535).
** \param     value Reference where to store the value of the holding register.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::readHoldingReg(uint16_t  addr, 
                                               uint16_t& value)
{
  return TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
} /*** end of readHoldingReg ***/


/************************************************************************************//**
** \brief     Writes a data element to the holding registers data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \attention The value of the holding register in already in your CPUs native endianess.
** \param     addr Element address (0..65535).
** \param     value Value of the holding register.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::writeHoldingReg(uint16_t addr,
                                                uint16_t  value)
{
  return TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
} /*** end of writeHoldingReg ***/


/************************************************************************************//**
** \brief     Implements custom function code handling for supporting Modbus function
**            codes that are either currently not supported or user defined extensions.
** \details   The "rxPdu" and "txPdu" parameters are byte arrays of the PDU. The first
**            byte (i.e. rxPdu[0]) contains the function code, followed by its data
**            bytes. Upon calling the callback, the "len" parameter contains the length
**            of "rxPdu". When preparing the response, you can write the length of the
**            "txPdu" response to "len" as well.
** \param     rxPdu Byte array for reading the received PDU.
** \param     txPdu Byte array for writing the response PDU.
** \param     len Reference to the PDU length, including the function code.
** \return    True if the callback function handled the received function code and 
**            prepared a response PDU, false otherwise.
**
****************************************************************************************/
bool TbxMbServer::customFunction(uint8_t  const rxPdu[], 
                                 uint8_t        txPdu[],
                                 uint8_t&       len)
{
  return false;
} /*** end of customFunction ***/


/************************************************************************************//**
** \brief     Wrapper to connect this callback to the readInput() method of a class
**            instance.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the discrete input to. Use TBX_ON if
**            the input is on, TBX_OFF for off.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::callbackReadInput(tTbxMbServer   channel, 
                                                   uint16_t       addr, 
                                                   uint8_t      * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;

  /* Only continue with a valid opaque channel pointer and value pointer. */
  if ( (channel != nullptr) && (value != nullptr) )
  {
    /* Convert the opaque pointer to the channel context structure pointer. */
    ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(channel);
    /* Only continue with a valid instance pointer. */
    if (channelCtx->instancePtr != nullptr)
    {
      /* The channel's instance pointer points to an instance of this class. Cast it as
       * such.
       */
      TbxMbServer * serverPtr = static_cast<TbxMbServer *>(channelCtx->instancePtr);
      /* Call the related instance method. */
      bool inputValue = false;
      result = serverPtr->readInput(addr, inputValue);
      /* Process the result. */
      if (result == TBX_MB_SERVER_OK)
      {
        *value = (inputValue == false) ? TBX_OFF : TBX_ON;
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of callbackReadInput ***/


/************************************************************************************//**
** \brief     Wrapper to connect this callback to the readCoil() method of a class
**            instance.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the coil to. Use TBX_ON if the coil
**            is on, TBX_OFF for off.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::callbackReadCoil(tTbxMbServer   channel, 
                                                 uint16_t       addr, 
                                                 uint8_t      * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;

  /* Only continue with a valid opaque channel pointer and value pointer. */
  if ( (channel != nullptr) && (value != nullptr) )
  {
    /* Convert the opaque pointer to the channel context structure pointer. */
    ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(channel);
    /* Only continue with a valid instance pointer. */
    if (channelCtx->instancePtr != nullptr)
    {
      /* The channel's instance pointer points to an instance of this class. Cast it as
       * such.
       */
      TbxMbServer * serverPtr = static_cast<TbxMbServer *>(channelCtx->instancePtr);
      /* Call the related instance method. */
      bool coilValue = false;
      result = serverPtr->readCoil(addr, coilValue);
      /* Process the result. */
      if (result == TBX_MB_SERVER_OK)
      {
        *value = (coilValue == false) ? TBX_OFF : TBX_ON;
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of callbackReadCoil ***/


/************************************************************************************//**
** \brief     Wrapper to connect this callback to the writeCoil() method of a class
**            instance.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Value of the coil. TBX_ON if the new coil state should be on,
**            TBX_OFF for off.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::callbackWriteCoil(tTbxMbServer channel, 
                                                  uint16_t     addr, 
                                                  uint8_t      value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;

  /* Only continue with a valid opaque channel pointer. */
  if (channel != nullptr)
  {
    /* Convert the opaque pointer to the channel context structure pointer. */
    ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(channel);
    /* Only continue with a valid instance pointer. */
    if (channelCtx->instancePtr != nullptr)
    {
      /* The channel's instance pointer points to an instance of this class. Cast it as
       * such.
       */
      TbxMbServer * serverPtr = static_cast<TbxMbServer *>(channelCtx->instancePtr);
      /* Call the related instance method. */
      bool coilValue = (value == TBX_OFF) ? false : true;
      result = serverPtr->writeCoil(addr, coilValue);
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of callbackWriteCoil ***/


/************************************************************************************//**
** \brief     Wrapper to connect this callback to the readInputReg() method of a class
**            instance.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the input register to.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::callbackReadInputReg(tTbxMbServer  channel, 
                                                    uint16_t       addr, 
                                                    uint16_t     * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;

  /* Only continue with a valid opaque channel pointer and value pointer. */
  if ( (channel != nullptr) && (value != nullptr) )
  {
    /* Convert the opaque pointer to the channel context structure pointer. */
    ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(channel);
    /* Only continue with a valid instance pointer. */
    if (channelCtx->instancePtr != nullptr)
    {
      /* The channel's instance pointer points to an instance of this class. Cast it as
       * such.
       */
      TbxMbServer * serverPtr = static_cast<TbxMbServer *>(channelCtx->instancePtr);
      /* Call the related instance method. */
      uint16_t inputRegValue = 0U;
      result = serverPtr->readInputReg(addr, inputRegValue);
      /* Process the result. */
      if (result == TBX_MB_SERVER_OK)
      {
        *value = inputRegValue;
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of callbackReadInputReg ***/


/************************************************************************************//**
** \brief     Wrapper to connect this callback to the readHoldingReg() method of a class
**            instance.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the holding register to.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::callbackReadHoldingReg(tTbxMbServer   channel, 
                                                       uint16_t       addr, 
                                                       uint16_t     * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;

  /* Only continue with a valid opaque channel pointer and value pointer. */
  if ( (channel != nullptr) && (value != nullptr) )
  {
    /* Convert the opaque pointer to the channel context structure pointer. */
    ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(channel);
    /* Only continue with a valid instance pointer. */
    if (channelCtx->instancePtr != nullptr)
    {
      /* The channel's instance pointer points to an instance of this class. Cast it as
       * such.
       */
      TbxMbServer * serverPtr = static_cast<TbxMbServer *>(channelCtx->instancePtr);
      /* Call the related instance method. */
      uint16_t holdingRegValue = 0U;
      result = serverPtr->readHoldingReg(addr, holdingRegValue);
      /* Process the result. */
      if (result == TBX_MB_SERVER_OK)
      {
        *value = holdingRegValue;
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of callbackReadHoldingReg ***/


/************************************************************************************//**
** \brief     Wrapper to connect this callback to the writeHoldingReg() method of a class
**            instance.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value New value of the holding register.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult TbxMbServer::callbackWriteHoldingReg(tTbxMbServer channel, 
                                                        uint16_t     addr, 
                                                        uint16_t     value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;

  /* Only continue with a valid opaque channel pointer. */
  if (channel != nullptr)
  {
    /* Convert the opaque pointer to the channel context structure pointer. */
    ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(channel);
    /* Only continue with a valid instance pointer. */
    if (channelCtx->instancePtr != nullptr)
    {
      /* The channel's instance pointer points to an instance of this class. Cast it as
       * such.
       */
      TbxMbServer * serverPtr = static_cast<TbxMbServer *>(channelCtx->instancePtr);
      /* Call the related instance method. */
      result = serverPtr->writeHoldingReg(addr, value);
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of callbackWriteHoldingReg ***/


/************************************************************************************//**
** \brief     Wrapper to connect this callback to the customFunction() method of a class
**            instance.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     rxPdu Pointer to a byte array for reading the received PDU.
** \param     txPdu Pointer to a byte array for writing the response PDU.
** \param     len Pointer to the PDU length, including the function code.
** \return    TBX_TRUE if the callback function handled the received function code and 
**            prepared a response PDU. TBX_FALSE otherwise.
**
****************************************************************************************/
uint8_t TbxMbServer::calbackCustomFunction(tTbxMbServer         channel, 
                                           uint8_t      const * rxPdu,
                                           uint8_t            * txPdu,
                                           uint8_t            * len)
{
  uint8_t result = TBX_FALSE;

  /* Only continue with a valid opaque channel pointer and PDU and len pointers. */
  if ( (channel != nullptr) && (rxPdu != nullptr) && (txPdu != nullptr) && 
       (len != nullptr)) 
  {
    /* Convert the opaque pointer to the channel context structure pointer. */
    ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(channel);
    /* Only continue with a valid instance pointer. */
    if (channelCtx->instancePtr != nullptr)
    {
      /* The channel's instance pointer points to an instance of this class. Cast it as
       * such.
       */
      TbxMbServer * serverPtr = static_cast<TbxMbServer *>(channelCtx->instancePtr);
      /* Call the related instance method. */
      if (serverPtr->customFunction(rxPdu, txPdu, *len))
      {
        /* Update the result. */
        result = TBX_TRUE;
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of calbackCustomFunction ***/


/****************************************************************************************
*                            T B X M B S E R V E R R T U
****************************************************************************************/
/************************************************************************************//**
** \brief     Modbus RTU server constructor.
** \param     nodeAddr The address of the node. Can be in the range 1..247 for a server
**            node. Set it to 0 for the client.
** \param     port The serial port to use. The actual meaning of the serial port is
**            hardware dependent. It typically maps to the UART peripheral number. E.g. 
**            TBX_MB_UART_PORT1 = USART1 on an STM32.
** \param     baudrate The desired communication speed.
** \param     stopbits Number of stop bits at the end of a character.
** \param     parity Parity bit type to use.
**
****************************************************************************************/
TbxMbServerRtu::TbxMbServerRtu(uint8_t            nodeAddr, 
                               tTbxMbUartPort     serialPort, 
                               tTbxMbUartBaudrate baudrate,
                               tTbxMbUartStopbits stopbits,
                               tTbxMbUartParity   parity)
  : TbxMbServer(), m_Transport(nullptr)
{
  /* Create the Modbus RTU transport layer object. */
  m_Transport = TbxMbRtuCreate(nodeAddr, serialPort, baudrate, stopbits, parity);
  /* Make sure the transport layer object could be created. */
  TBX_ASSERT(m_Transport != nullptr);

  /* Only continue with a valid transport layer object. */
  if (m_Transport != nullptr)
  {
    /* Create a Modbus server channel object and link the RTU transport layer object. */
    m_Channel = TbxMbServerCreate(m_Transport);
    /* Make sure the server channel object could be created. */
    TBX_ASSERT(m_Channel != nullptr);

    /* Only continue with a valid server channel object. */
    if (m_Channel != nullptr)
    {
      /* Convert the opaque pointer to the channel context structure pointer. */
      ChannelCtx * channelCtx = reinterpret_cast<ChannelCtx *>(m_Channel);
      /* Store our instance pointer in the channel context. Needed for binding the 
       * callback functions to instance methods.
       */
      channelCtx->instancePtr = this;
      /* Register the callback functions. */
      TbxMbServerSetCallbackReadInput(m_Channel, callbackReadInput);
      TbxMbServerSetCallbackReadCoil(m_Channel, callbackReadCoil);
      TbxMbServerSetCallbackWriteCoil(m_Channel, callbackWriteCoil);
      TbxMbServerSetCallbackReadInputReg(m_Channel, callbackReadInputReg);
      TbxMbServerSetCallbackReadHoldingReg(m_Channel, callbackReadHoldingReg);
      TbxMbServerSetCallbackWriteHoldingReg(m_Channel, callbackWriteHoldingReg);
      TbxMbServerSetCallbackCustomFunction(m_Channel, calbackCustomFunction);
    }
  }
} /*** end of TbxMbServerRtu ***/


/************************************************************************************//**
** \brief     Modbus RTU server destructor.
**
****************************************************************************************/
TbxMbServerRtu::~TbxMbServerRtu()
{
  /* Server object valid? */
  if (m_Channel != nullptr)
  {
    /* Release the server object. */
    TbxMbServerFree(m_Channel);
  }
  /* Transport layer object valid? */
  if (m_Transport != nullptr)
  {
    /* Release the transport layer object. */
    TbxMbRtuFree(m_Transport);
  }
} /*** end of ~TbxMbServerRtu ***/


/*********************************** end of tbxmbserver.cpp ****************************/
