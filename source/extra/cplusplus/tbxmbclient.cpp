/************************************************************************************//**
* \file         tbxmbclient.cpp
* \brief        MicroTBX-Modbus client C++ source file.
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
*                            T B X M B C L I E N T
****************************************************************************************/
/************************************************************************************//**
** \brief     Modbus client base destructor.
**
****************************************************************************************/
TbxMbClient::~TbxMbClient()
{
  /* Nothing to do here but do not remove this destructor. The destructor was made pure
   * virtual to force the class to be abstract. This was needed because it doesn't
   * contain any pure virtual methods. 
   * Note that the destructor is the only method that, even if it is pure virtual, has to 
   * have an implementation. Otherwise derived classes cannot be instantiated.
   */
} /*** end of ~TbxMbClient ***/


/************************************************************************************//**
** \brief     Reads the coil(s) from the server with the specified node address.
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     addr Starting element address (0..65535) in the Modbus data table for the
**            coil read operation.
** \param     num Number of elements to read from the coils data table. Range can be
**            1..2000
** \param     coils Byte array with TBX_ON / TBX_OFF values where the coil state will be
**            written to.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::readCoils(uint8_t  node, 
                               uint16_t addr, 
                               uint16_t num, 
                               uint8_t  coils[])
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientReadCoils(m_Channel, node, addr, num, coils);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of readCoils ***/


/************************************************************************************//**
** \brief     Reads the discrete input(s) from the server with the specified node
**            address.
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     addr Starting element address (0..65535) in the Modbus data table for the
**            discrete input read operation.
** \param     num Number of elements to read from the discrete inputs data table. Range
**            can be 1..2000
** \param     inputs Byte array with TBX_ON / TBX_OFF values where the discrete
**            input state will be written to.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::readInputs(uint8_t  node, 
                                uint16_t addr,
                                uint16_t num,
                                uint8_t  inputs[])
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientReadInputs(m_Channel, node, addr, num, inputs);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of readInputs ***/


/************************************************************************************//**
** \brief     Reads the input register(s) from the server with the specified node
**            address.
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     addr Starting element address (0..65535) in the Modbus data table for the
**            input register read operation.
** \param     num Number of elements to read from the input registers data table. Range
**            can be 1..125
** \param     inputRegs Array where the input register values will be written to.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::readInputRegs(uint8_t  node, 
                                   uint16_t addr, 
                                   uint8_t  num, 
                                   uint16_t inputRegs[])
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientReadInputRegs(m_Channel, node, addr, num, inputRegs);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of readInputRegs ****/


/************************************************************************************//**
** \brief     Reads the holding register(s) from the server with the specified node
**            address.
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     addr Starting element address (0..65535) in the Modbus data table for the
**            holding register read operation.
** \param     num Number of elements to read from the holding registers data table. Range
**            can be 1..125
** \param     holdingRegs Array where the holding register values will be written to.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::readHoldingRegs(uint8_t  node, 
                                     uint16_t addr,
                                     uint8_t  num,
                                     uint16_t holdingRegs[])
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientReadHoldingRegs(m_Channel, node, addr, num, holdingRegs);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of readHoldingRegs ***/


/************************************************************************************//**
** \brief     Writes the coil(s) to the server with the specified node address.
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     addr Starting element address (0..65535) in the Modbus data table for the
**            coil write operation.
** \param     num Number of elements to write to the coils data table. Range can be
**            1..1968
** \param     coils Array with the desired TBX_ON / TBX_OFF coils values.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::writeCoils(uint8_t       node, 
                                uint16_t      addr, 
                                uint16_t      num, 
                                uint8_t const coils[])
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientWriteCoils(m_Channel, node, addr, num, coils);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of writeCoils ***/


/************************************************************************************//**
** \brief     Writes the holding register(s) to the server with the specified node
**            address.
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     addr Starting element address (0..65535) in the Modbus data table for the
**            holding register write operation.
** \param     num Number of elements to write to the holding registers data table. Range
**            can be 1..123
** \param     holdingRegs Array with the desired holding register values.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::writeHoldingRegs(uint8_t         node, 
                                      uint16_t        addr, 
                                      uint8_t         num, 
                                      uint16_t const  holdingRegs[])
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientWriteHoldingRegs(m_Channel, node, addr, num, holdingRegs);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of writeHoldingRegs ***/


/************************************************************************************//**
** \brief     Perform diagnostic operation on the server for checking the communication
**            system.
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     subcode Sub-function code for specifying the diagnostic operation to
**            perform. Currently supported values:
**              - TBX_MB_DIAG_SC_QUERY_DATA
**              - TBX_MB_DIAG_SC_CLEAR_COUNTERS
**              - TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT
**              - TBX_MB_DIAG_SC_BUS_COMM_ERROR_COUNT
**              - TBX_MB_DIAG_SC_BUS_EXCEPTION_ERROR_COUNT
**              - TBX_MB_DIAG_SC_SERVER_MESSAGE_COUNT
**              - TBX_MB_DIAG_SC_SERVER_NO_RESPONSE_COUNT
** \param     count Reference to where the retrieved count value will be written to. Only
**            applicable for the subcodes that end with _COUNT.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::diagnostics(uint8_t node, uint16_t subcode, uint16_t& count)
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientDiagnostics(m_Channel, node, subcode, &count);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of diagnostics ***/


/************************************************************************************//**
** \brief     Send a custom function code PDU to the server and receive its response PDU.
**            Thanks to this functionality, the user can support Modbus function codes
**            that are either currently not supported or user defined extensions.
** \details   The "txPdu" and "rxPdu" parameters are byte arrays of the PDU. The first
**            byte (i.e. txPdu[0]) contains the function code, followed by its data
**            bytes. When calling this function, set the "len" parameter to the length of
**            the "txPdu". This function updates the "len" parameter with the length of
**            the received PDU, which it stores in "rxPdu".
** \example   Example of manually sending the "Write Single Register 0x06" function code
**            to a node with address 10 (0x0A) for setting the holding register at
**            address 40000 to a value of 127:
**
**              uint8_t  requestPdu[TBX_MB_TP_PDU_MAX_LEN];
**              uint8_t  responsePdu[TBX_MB_TP_PDU_MAX_LEN];
**              uint8_t  pduLen;
**              uint16_t holdingRegAddr = 40000U;
**              uint16_t holdingRegValue = 127U;
**
**              requestPdu[0] = TBX_MB_FC06_WRITE_SINGLE_REGISTER;
**              requestPdu[1] = (uint8_t)(holdingRegAddr >> 8U);
**              requestPdu[2] = (uint8_t) holdingRegAddr;
**              requestPdu[3] = (uint8_t)(holdingRegValue >> 8U);
**              requestPdu[4] = (uint8_t) holdingRegValue;
**              pduLen = 5U;
**
**              myModbusClient.customFunction(0x0A, requestPdu, responsePdu, pduLen);
**
** \param     node The address of the server. This parameter is transport layer
**            dependent. It is needed on RTU/ASCII, yet don't care for TCP unless it is
**            a gateway to an RTU network. If it's don't care, set it to a value of 1.
** \param     txPdu Byte array with the PDU to transmit.
** \param     rxPdu Byte array with the received response PDU.
** \param     len Reference to the PDU length, including the function code.
** \return    TBX_OK if successful, TBX_ERROR otherwise.
**
****************************************************************************************/
uint8_t TbxMbClient::customFunction(uint8_t       node, 
                                    uint8_t const txPdu[], 
                                    uint8_t       rxPdu[], 
                                    uint8_t&      len)
{
  uint8_t result = TBX_ERROR;

  /* Only continue with a valid client object. */
  if (m_Channel != nullptr)
  {
    result = TbxMbClientCustomFunction(m_Channel, node, txPdu, rxPdu, &len);
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of customFunction ***/


/****************************************************************************************
*                            T B X M B C L I E N T R T U
****************************************************************************************/
/************************************************************************************//**
** \brief     Modbus RTU client constructor.
** \param     responseTimeout Maximum time in milliseconds to wait for a response from
**            the Modbus server, after sending a PDU.
** \param     turnaroundDelay Delay time in milliseconds after sending a broadcast PDU
**            to give all recipients sufficient time to process the PDU.
** \param     port The serial port to use. The actual meaning of the serial port is
**            hardware dependent. It typically maps to the UART peripheral number. E.g. 
**            TBX_MB_UART_PORT1 = USART1 on an STM32.
** \param     baudrate The desired communication speed.
** \param     stopbits Number of stop bits at the end of a character.
** \param     parity Parity bit type to use.
**
****************************************************************************************/
TbxMbClientRtu::TbxMbClientRtu(uint16_t           responseTimeout, 
                               uint16_t           turnaroundDelay, 
                               tTbxMbUartPort     serialPort, 
                               tTbxMbUartBaudrate baudrate,
                               tTbxMbUartStopbits stopbits,
                               tTbxMbUartParity   parity)
  : TbxMbClient(), m_Transport(nullptr)
{
  /* Create the Modbus RTU transport layer object. Node address should be set to 0 for
   * a client.
   */
  m_Transport = TbxMbRtuCreate(0U, serialPort, baudrate, stopbits, parity);
  /* Make sure the transport layer object could be created. */
  TBX_ASSERT(m_Transport != nullptr);

  /* Only continue with a valid transport layer object. */
  if (m_Transport != nullptr)
  {
    /* Create a Modbus client channel object and link the RTU transport layer object. */
    m_Channel = TbxMbClientCreate(m_Transport, responseTimeout, turnaroundDelay);
    /* Make sure the client channel object could be created. */
    TBX_ASSERT(m_Channel != nullptr);
  }
} /*** end of TbxMbClientRtu ***/


/************************************************************************************//**
** \brief     Modbus RTU client destructor.
**
****************************************************************************************/
TbxMbClientRtu::~TbxMbClientRtu()
{
  /* Client object valid? */
  if (m_Channel != nullptr)
  {
    /* Release the client object. */
    TbxMbClientFree(m_Channel);
  }
  /* Transport layer object valid? */
  if (m_Transport != nullptr)
  {
    /* Release the transport layer object. */
    TbxMbRtuFree(m_Transport);
  }
} /*** end of ~TbxMbClientRtu ***/


/*********************************** end of tbxmbclient.cpp ****************************/
