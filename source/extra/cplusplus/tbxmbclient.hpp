/************************************************************************************//**
* \file         tbxmbclient.hpp
* \brief        MicroTBX-Modbus client C++ header file.
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
#ifndef TBXMBCLIENT_HPP
#define TBXMBCLIENT_HPP

/****************************************************************************************
* Class definitions
****************************************************************************************/
/****************************************************************************************
*                            T B X M B C L I E N T
****************************************************************************************/
/** \brief Abstract Modbus client base class. */
class TbxMbClient
{
public:
  /* Constructors and destructor. */
  TbxMbClient() : m_Channel(nullptr) { }
  virtual ~TbxMbClient() = 0;
  /* Methods. */
  uint8_t readCoils(uint8_t node, uint16_t addr, uint16_t num, uint8_t coils[]);
  uint8_t readInputs(uint8_t node, uint16_t addr, uint16_t num, uint8_t inputs[]);
  uint8_t readInputRegs(uint8_t node, uint16_t addr, uint8_t num, uint16_t inputRegs[]);
  uint8_t readHoldingRegs(uint8_t node, uint16_t addr, uint8_t num, 
                          uint16_t holdingRegs[]);
  uint8_t writeCoils(uint8_t node, uint16_t addr, uint16_t num, uint8_t const coils[]);
  uint8_t writeHoldingRegs(uint8_t node, uint16_t addr, uint8_t num, 
                           uint16_t const holdingRegs[]);
  uint8_t diagnostics(uint8_t node, uint16_t subcode, uint16_t& count);
  uint8_t customFunction(uint8_t node, uint8_t const txPdu[], uint8_t rxPdu[],
                         uint8_t& len);

protected:
  /* Members. */
  tTbxMbClient m_Channel;

};


/****************************************************************************************
*                            T B X M B C L I E N T R T U
****************************************************************************************/
/** \brief Modbus client class that uses RTU as the transport layer. */
class TbxMbClientRtu : public TbxMbClient
{
public:
  /* Constructors and destructor. */
  TbxMbClientRtu(uint16_t responseTimeout, uint16_t turnaroundDelay, 
                 tTbxMbUartPort serialPort, tTbxMbUartBaudrate baudrate, 
                 tTbxMbUartStopbits stopbits, tTbxMbUartParity parity);
  TbxMbClientRtu(tTbxMbUartPort serialPort, tTbxMbUartBaudrate baudrate, 
                 tTbxMbUartStopbits stopbits, tTbxMbUartParity parity)
    : TbxMbClientRtu(1000U, 100U, serialPort, baudrate, stopbits, parity) { }
  virtual ~TbxMbClientRtu();

private:
  /* Members.*/
  tTbxMbTp m_Transport;
};

#endif /* TBXMBCLIENT_HPP */
/*********************************** end of tbxmbclient.hpp ****************************/

