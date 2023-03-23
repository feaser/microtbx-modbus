/************************************************************************************//**
* \file         tbxmbserver.hpp
* \brief        MicroTBX-Modbus server C++ header file.
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
#ifndef TBXMBSERVER_HPP
#define TBXMBSERVER_HPP

/****************************************************************************************
* Class definitions
****************************************************************************************/
/****************************************************************************************
*                            T B X M B S E R V E R
****************************************************************************************/
/** \brief Abstract Modbus server base class. */
class TbxMbServer
{
public:
  /* Constructors and destructor. */
  TbxMbServer() : m_Channel(nullptr) { }
  virtual ~TbxMbServer() = 0;

private:
  /* Methods. */
  virtual tTbxMbServerResult readInput(uint16_t addr, bool& value);
  virtual tTbxMbServerResult readCoil(uint16_t addr, bool& value);
  virtual tTbxMbServerResult writeCoil(uint16_t addr, bool value);
  virtual tTbxMbServerResult readInputReg(uint16_t addr, uint16_t& value);
  virtual tTbxMbServerResult readHoldingReg(uint16_t addr, uint16_t& value);
  virtual tTbxMbServerResult writeHoldingReg(uint16_t addr, uint16_t value);
  virtual bool               customFunction(uint8_t const rxPdu[], uint8_t txPdu[], 
                                            uint8_t& len);

protected:
  /* Types. */
  using ChannelCtx = struct
  {
    void * instancePtr;
  };
  /* Members. */
  tTbxMbServer m_Channel;
  /* Callbacks. */
  static tTbxMbServerResult callbackReadInput(tTbxMbServer channel, uint16_t addr, 
                                               uint8_t * value);
  static tTbxMbServerResult callbackReadCoil(tTbxMbServer channel, uint16_t addr, 
                                             uint8_t * value);
  static tTbxMbServerResult callbackWriteCoil(tTbxMbServer channel, uint16_t addr, 
                                              uint8_t value);
  static tTbxMbServerResult callbackReadInputReg(tTbxMbServer channel, uint16_t addr, 
                                                  uint16_t * value);
  static tTbxMbServerResult callbackReadHoldingReg(tTbxMbServer channel, uint16_t addr, 
                                                   uint16_t * value);
  static tTbxMbServerResult callbackWriteHoldingReg(tTbxMbServer channel, uint16_t addr, 
                                                    uint16_t value);
  static  uint8_t           calbackCustomFunction(tTbxMbServer channel,
                                                  uint8_t const * rxPdu, uint8_t * txPdu,
                                                  uint8_t * len);
};


/****************************************************************************************
*                            T B X M B S E R V E R R T U
****************************************************************************************/
/** \brief Modbus server class that uses RTU as the transport layer. */
class TbxMbServerRtu : public TbxMbServer
{
public:
  /* Constructors and destructor. */
  TbxMbServerRtu(uint8_t nodeAddr, tTbxMbUartPort serialPort, 
                 tTbxMbUartBaudrate baudrate, tTbxMbUartStopbits stopbits,
                 tTbxMbUartParity parity);
  virtual ~TbxMbServerRtu();

private:
  /* Members.*/
  tTbxMbTp m_Transport;
};

#endif /* TBXMBSERVER_HPP */
/*********************************** end of tbxmbserver.hpp ****************************/

