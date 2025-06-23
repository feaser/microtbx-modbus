/************************************************************************************//**
* \file         unittests.c
* \brief        Unit tests source file.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2025 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
*
* SPDX-License-Identifier: MIT
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* \endinternal
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "microtbx.h"                            /* MicroTBX global header             */
#include "microtbxmodbus.h"                      /* MicroTBX-Modbus global header      */
#include "unity.h"                               /* Unity unit test framework          */
#include "unittests.h"                           /* Unit tests header                  */
#include <sys/time.h>                            /* time definitions                   */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Keeps track of how often an assertion got triggered. */
uint32_t assertionCnt = 0;

/** \brief Modbus server coils. */
uint8_t mbServerCoils[2] = { TBX_OFF, TBX_OFF };

/** \brief Modbus server holding registers. */
uint16_t mbServerHoldingRegs[2] = { 0x789AU, 0xA51FU };

/** \brief An invalid MicroTBX-Modbus context. The type is set to one that is not used
 *         by any of its internal contexts. 
 */
struct
{
  void    * instancePtr;                         /**< Reserved for C++ wrapper.        */
  void    * pollFcn;                             /**< Event poll function.             */
  void    * processFcn;                          /**< Event process function.          */
  /* The type member must always be the first one after the three entries that match
   * those in tTbxMbEventCtx.
   */
  uint8_t   type;                                /**< Context type.                    */
} invalidCtx = {NULL, NULL, NULL, 255U};


/************************************************************************************//**
** \brief     Reads a data element from the discrete inputs data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the input to. Use TBX_ON if the input
**            is on, TBX_OFF otherwise.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult mbServer_ReadInput(tTbxMbServer channel, uint16_t addr,
                                      uint8_t * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_OK;

  /* Filter on the requested discrete input address. */
  switch (addr)
  {
  case 10000U:
    *value = TBX_ON;
    break;

  case 10001U:
    *value = TBX_OFF;
    break;

  default:
  /* Unsupported discrete input address. */
  result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
  break;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of mbServer_ReadInput ***/


/************************************************************************************//**
** \brief     Reads a data element from the coils data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the coil to. TBX_ON if the coil is
**            activate, TBX_OFF otherwise.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult mbServer_ReadCoil(tTbxMbServer channel, uint16_t addr,
                                            uint8_t * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_OK;

  TBX_UNUSED_ARG(channel);

  /* Filter on the requested coil address. */
  switch (addr)
  {
  case 0U:
    *value = mbServerCoils[0];
    break;

  case 1U:
    *value = mbServerCoils[1];
    break;

  default:
    /* Unsupported coil address. */
    result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
    break;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of mbServer_ReadCoil ***/


/************************************************************************************//**
** \brief     Writes a data element to the coils data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Coil value. Use TBX_ON to activate the coil, TBX_OFF otherwise.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult mbServer_WriteCoil(tTbxMbServer channel, uint16_t addr,
                                             uint8_t value)
{
tTbxMbServerResult result = TBX_MB_SERVER_OK;

TBX_UNUSED_ARG(channel);

  /* Filter on the requested coil address. */
  switch (addr)
  {
  case 0U:
    mbServerCoils[0] = value;
    break;

  case 1U:
    mbServerCoils[1] = value;
    break;

  default:
    /* Unsupported coil address. */
    result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
    break;
  }

/* Give the result back to the caller. */
return result;
} /*** end of mbServer_WriteCoil ***/


/************************************************************************************//**
** \brief     Reads a data element from the input registers data table.
** \details   Write the value of the input register in your CPUs native endianess. The
**            MicroTBX-Modbus stack will automatically convert this to the big endianess
**            that the Modbus protocol requires.
**            Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the input register to.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult mbServer_ReadInputReg(tTbxMbServer channel, uint16_t addr, 
                                         uint16_t * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_OK;

  TBX_UNUSED_ARG(channel);

  /* Filter on the requested input register address. */
  switch (addr)
  {
  case 30000U:
    *value = 0x1234U;
    break;

  case 30001U:
    *value = 0x5678U;
    break;

  default:
    /* Unsupported input register address. */
    result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
    break;
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of mbServer_ReadInputReg ***/


/************************************************************************************//**
** \brief     Reads a data element from the holding registers data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
**            The value of the holding register in already in your CPUs native endianess.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Pointer to write the value of the holding register to.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult mbServer_ReadHoldingReg(tTbxMbServer channel, uint16_t addr, 
                                           uint16_t * value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_OK;

  TBX_UNUSED_ARG(channel);

  /* Filter on the requested holding register address. */
  switch (addr)
  {
  case 40000U:
    *value = mbServerHoldingRegs[0];
    break;

  case 40001U:
    *value = mbServerHoldingRegs[1];
    break;

  default:
    /* Unsupported holding register address. */
    result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
    break;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of mbServer_ReadHoldingReg ***/


/************************************************************************************//**
** \brief     Writes a data element to the holding registers data table.
** \details   Note that the element is specified by its zero-based address in the range
**            0 - 65535, not its element number (1 - 65536).
**            The value of the holding register in already in your CPUs native endianess.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     addr Element address (0..65535).
** \param     value Value of the holding register.
** \return    TBX_MB_SERVER_OK if successful, TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR if the
**            specific data element address is not supported by this server, 
**            TBX_MB_SERVER_ERR_DEVICE_FAILURE otherwise.
**
****************************************************************************************/
tTbxMbServerResult mbServer_WriteHoldingReg(tTbxMbServer channel, uint16_t addr, 
                                            uint16_t value)
{
  tTbxMbServerResult result = TBX_MB_SERVER_OK;

  TBX_UNUSED_ARG(channel);

  /* Filter on the requested holding register address. */
  switch (addr)
  {
  case 40000U:
    /* This holding register supports < 1024 values. */
    if (value < 1024U)
    {
      mbServerHoldingRegs[0] = value;
    }
    else
    {
      result = TBX_MB_SERVER_ERR_DEVICE_FAILURE;
    }
    break;

  case 40001U:
    mbServerHoldingRegs[1] = value;
    break;

  default:
    /* Unsupported holding register address. */
    result = TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR;
    break;
  }

  /* Give the result back to the caller. */
  return result;
} /*** end of mbServer_WriteHoldingReg ***/



/************************************************************************************//**
** \brief     Custom function code implementation for function code 17 (Report ServerID).
** \details   According to the Modbus protocol, the response to the Report Server ID 
**            request is device specific. The device implementation decides the number of
**            bytes for the Server ID and if additional data is added to the response. 
**            The following code snippet implements support for Report Server ID, where
**            the actual server ID is 16-bits and the response contains no additional
**            data.
** \param     channel Handle to the Modbus server channel object that triggered the 
**            callback.
** \param     rxPdu Pointer to a byte array for reading the received PDU.
** \param     txPdu Pointer to a byte array for writing the response PDU.
** \param     len Pointer to the received PDU length, including the function code. When
**            preparing the response, you can write the length of the transmit PDU to
**            len as well.
** \return    TBX_TRUE if the callback function handled the received function code and
**            prepared a response PDU. TBX_FALSE otherwise.
**
****************************************************************************************/
uint8_t mbServer_ReportServerIdCallback(tTbxMbServer channel, uint8_t const * rxPdu,
                                        uint8_t * txPdu, uint8_t * len)
{
  uint8_t result = TBX_FALSE;

  /* Function code 17 - Report Server ID? */
  if (rxPdu[0] == 17U)
  {
    /* Check the expected request length. */
    if (*len == 1U)
    {
      /* Prepare the response. */
      txPdu[0] = 17U; /* Function code. */
      txPdu[1] = 3U;  /* Byte count. */
      TbxMbCommonStoreUInt16BE(0x1234U, &txPdu[2]); /* server ID. */
      txPdu[4] = 0xFFU; /* Run indicator status = ON. */
      *len = 5U;
      /* Function code handled. */
      result = TBX_TRUE;
    }
  }

  /* Give the result back to the caller. */
  return result;
}  /*** end of mbServer_ReportServerIdCallback ***/ 


/************************************************************************************//**
** \brief     In the simulated environment with both a Modbus server and client, the 
**            event task is not called continuously, but only when the client is waiting
**            for a response. To be able to perform run-time client-server interactions,
**            both their transport layers need to be in the operational state. For this
**            reason, the event task should run for a bit before the client attempts to
**            communicate with the server.
**
****************************************************************************************/
void startupModbusStack(void)
{
  /* Run the Modbus stack about 150ms.*/ 
  uint16_t startTicks = TbxMbPortTimerCount();
  uint16_t deltaTicks;
  do
  {
    /* Run the Modbus stack. */
    TbxMbEventTask();
    /* Sleep a bit to not starve the CPU. */
    //(void)usleep(1000U);    
    /* Calculate number of 50us ticks that elapsed since the start of the loop. */
    deltaTicks = TbxMbPortTimerCount() - startTicks;
  } 
  while (deltaTicks <= 3000U);
} /*** end of startupModbusStack ***/


/************************************************************************************//**
** \brief     Handles the run-time assertions. 
** \param     file The filename of the source file where the assertion occurred in.
** \param     line The line number inside the file where the assertion occurred.
**
****************************************************************************************/
void handleTbxAssertion(const char * const file, uint32_t line)
{
  /* Increment the assertion counter. */
  assertionCnt++;
} /*** end of handleTbxAssertion ***/


/************************************************************************************//**
** \brief     Tests that verifies that the version macros are present.
**
****************************************************************************************/
void test_TbxMbGeneric_VersionMacrosShouldBePresent(void)
{
  /* Check presence of the main version macro. */
  #ifndef TBX_MB_VERSION_MAIN
  TEST_FAIL();
  #endif
  /* Check presence of the minor version macro. */
  #ifndef TBX_MB_VERSION_MINOR
  TEST_FAIL();
  #endif
  /* Check presence of the patch version macro. */
  #ifndef TBX_MB_VERSION_PATCH
  TEST_FAIL();
  #endif
} /*** end of test_TbxMbGeneric_VersionMacrosShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that verifies that the function code macros are present.
**
****************************************************************************************/
void test_TbxMbGeneric_FunctionCodeMacrosShouldBePresent(void)
{
  #ifndef TBX_MB_FC01_READ_COILS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC02_READ_DISCRETE_INPUTS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC03_READ_HOLDING_REGISTERS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC04_READ_INPUT_REGISTERS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC05_WRITE_SINGLE_COIL
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC06_WRITE_SINGLE_REGISTER
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC08_DIAGNOSTICS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC15_WRITE_MULTIPLE_COILS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_FC16_WRITE_MULTIPLE_REGISTERS
  TEST_FAIL();
  #endif
} /*** end of test_TbxMbGeneric_FunctionCodeMacrosShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that verifies that the exception code macros are present.
**
****************************************************************************************/
void test_TbxMbGeneric_ExceptionCodeMacrosShouldBePresent(void)
{
  #ifndef TBX_MB_EC01_ILLEGAL_FUNCTION
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_EC02_ILLEGAL_DATA_ADDRESS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_EC03_ILLEGAL_DATA_VALUE
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_EC04_SERVER_DEVICE_FAILURE
  TEST_FAIL();
  #endif
} /*** end of test_TbxMbGeneric_ExceptionCodeMacrosShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that verifies that the diagnostic sub code macros are present.
**
****************************************************************************************/
void test_TbxMbGeneric_DiagnosticSubCodeMacrosShouldBePresent(void)
{
  #ifndef TBX_MB_DIAG_SC_QUERY_DATA
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_DIAG_SC_CLEAR_COUNTERS
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_DIAG_SC_BUS_COMM_ERROR_COUNT
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_DIAG_SC_BUS_EXCEPTION_ERROR_COUNT
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_DIAG_SC_SERVER_MESSAGE_COUNT
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_DIAG_SC_SERVER_NO_RESPONSE_COUNT
  TEST_FAIL();
  #endif
} /*** end of test_TbxMbGeneric_DiagnosticSubCodeMacrosShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that verifies that the miscellaneous macros are present.
**
****************************************************************************************/
void test_TbxMbGeneric_MiscellaneousMacrosShouldBePresent(void)
{
  #ifndef TBX_MB_FC_EXCEPTION_MASK
  TEST_FAIL();
  #endif
} /*** end of test_TbxMbGeneric_MiscellaneousMacrosShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that verifies that the transport layer node address macros are
**            present.
**
****************************************************************************************/
void test_TbxMbGeneric_TpNodeAddressMacrosShouldBePresent(void)
{
  #ifndef TBX_MB_TP_NODE_ADDR_BROADCAST
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_TP_NODE_ADDR_MIN
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_TP_NODE_ADDR_MAX
  TEST_FAIL();
  #endif
} /*** end of test_TbxMbGeneric_TpNodeAddressMacrosShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that verifies that the transport layer protocol data unit macros are
**            present.
**
****************************************************************************************/
void test_TbxMbGeneric_TpPduMacrosShouldBePresent(void)
{
  #ifndef TBX_MB_TP_PDU_CODE_LEN_MAX
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_TP_PDU_DATA_LEN_MAX
  TEST_FAIL();
  #endif
  #ifndef TBX_MB_TP_PDU_MAX_LEN
  TEST_FAIL();
  #endif
} /*** end of test_TbxMbGeneric_TpPduMacrosShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that verifies that the types are present.
**
****************************************************************************************/
void test_TbxMbGeneric_TypesShouldBePresent(void)
{
  /* Server. */
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServer));
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerResult));
  TEST_ASSERT_EQUAL_INT(TBX_MB_SERVER_OK, 0U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR, 1U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_SERVER_ERR_DEVICE_FAILURE, 2U);
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerReadInput));
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerReadCoil));
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerWriteCoil));
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerReadInputReg));
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerReadHoldingReg));
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerWriteHoldingReg));
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbServerCustomFunction));
  /* Client. */
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbClient));
  /* Transport layer. */
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbTp));
  /* UART. */
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbUartPort));
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT1,    0U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT2,    1U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT3,    2U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT4,    3U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT5,    4U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT6,    5U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT7,    6U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_PORT8,    7U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_NUM_PORT, 8U);
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbUartBaudrate));
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_1200BPS,      0U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_2400BPS,      1U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_4800BPS,      2U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_9600BPS,      3U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_19200BPS,     4U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_38400BPS,     5U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_57600BPS,     6U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_115200BPS,    7U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_NUM_BAUDRATE, 8U);
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbUartDatabits));
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_7_DATABITS,   0U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_8_DATABITS,   1U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_NUM_DATABITS, 2U);
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbUartStopbits));
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_1_STOPBITS,   0U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_2_STOPBITS,   1U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_NUM_STOPBITS, 2U);
  TEST_ASSERT_GREATER_THAN_size_t(0U, sizeof(tTbxMbUartParity));
  TEST_ASSERT_EQUAL_INT(TBX_MB_ODD_PARITY,      0U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_EVEN_PARITY,     1U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_NO_PARITY,       2U);
  TEST_ASSERT_EQUAL_INT(TBX_MB_UART_NUM_PARITY, 3U);
} /*** end of test_TbxMbGeneric_TypesShouldBePresent ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns zero.
**
****************************************************************************************/
void test_TbxMbCommonExtractUInt16BE_ShouldAssertOnInvalidParams(void)
{
  uint16_t result;

  assertionCnt = 0;
  result = TbxMbCommonExtractUInt16BE(NULL);
  /* Make sure zero is returned. */
  TEST_ASSERT_EQUAL_UINT16(0, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxMbCommonExtractUInt16BE_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the 16-bit unsigned value can be extracted and returned in big
**            endian format. 
**
****************************************************************************************/
void test_TbxMbCommonExtractUInt16BE_CanExtract(void)
{
  uint16_t result;
  uint8_t input[2] = { 0xA5U, 0x1FU };

  assertionCnt = 0;
  result = TbxMbCommonExtractUInt16BE(input);
  /* Make sure the correct big endian 16-bit value is returned. */
  TEST_ASSERT_EQUAL_UINT16(0xA51FU, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxMbCommonExtractUInt16BE_CanExtract ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbCommonStoreUInt16BE_ShouldAssertOnInvalidParams(void)
{
  assertionCnt = 0;
  TbxMbCommonStoreUInt16BE(0U, NULL);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxMbCommonStoreUInt16BE_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the 16-bit unsigned value can be store in the byte array in big
**            endian format. 
**
****************************************************************************************/
void test_TbxMbCommonStoreUInt16BE_CanStore(void)
{
  uint16_t input = 0xA51FU;
  uint8_t  result[2] = { 0U, 0U };

  assertionCnt = 0;
  TbxMbCommonStoreUInt16BE(input, result);
  /* Make sure the correct big endian 16-bit value is stored in the array. */
  TEST_ASSERT_EQUAL_UINT8(0xA5U, result[0]);
  TEST_ASSERT_EQUAL_UINT8(0x1FU, result[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxMbCommonStoreUInt16BE_CanStore ***/


/************************************************************************************//**
** \brief     Tests that the event task can be called without triggering an assertion.
**
****************************************************************************************/
void test_TbxMbEventTask_CanCall(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First call it without any transport layers or channels created. */
  assertionCnt = 0;
  TbxMbEventTask();
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Now create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Now call it with a transport layer and channel created. */
  assertionCnt = 0;
  TbxMbEventTask();
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbEventTask_CanCall ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbUartTransmitComplete_ShouldAssertOnInvalidParams(void)
{
  assertionCnt = 0;
  TbxMbUartTransmitComplete(TBX_MB_UART_NUM_PORT);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxMbUartTransmitComplete_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbUartDataReceived_ShouldAssertOnInvalidParams(void)
{
  uint8_t dummyRxData[2] = { 0xAAU, 0x55U };

  assertionCnt = 0;
  TbxMbUartDataReceived(TBX_MB_UART_NUM_PORT, dummyRxData, 2U);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);

  assertionCnt = 0;
  TbxMbUartDataReceived(TBX_MB_UART_PORT1, NULL, 2U);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);

  assertionCnt = 0;
  TbxMbUartDataReceived(TBX_MB_UART_PORT1, dummyRxData, 0U);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
} /*** end of test_TbxMbUartDataReceived_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns NULL.
**
****************************************************************************************/
void test_TbxMbTpRtuCreate_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp result;
  size_t   heapFreeBefore;
  size_t   heapFreeAfter;

  /* Node address should be <= 247. Note the 0 is also allowed in case of a client. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbRtuCreate(248, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                          TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
                          heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, result);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Port should be a supported enum value. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbRtuCreate(10, TBX_MB_UART_NUM_PORT, TBX_MB_UART_19200BPS, 
                          TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
                          heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, result);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Baudrate should be a supported enum value. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_NUM_BAUDRATE, 
                          TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
                          heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, result);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Stopbits should be a supported enum value. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                          TBX_MB_UART_NUM_STOPBITS, TBX_MB_EVEN_PARITY);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, result);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
  
  /* Parity should be a supported enum value. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                          TBX_MB_UART_1_STOPBITS, TBX_MB_UART_NUM_PARITY);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, result);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbTpRtuCreate_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a RTU transport layer can be created.
**
****************************************************************************************/
void test_TbxMbTpRtuCreate_CanCreate(void)
{
  tTbxMbTp result;

  assertionCnt = 0;
  result = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                          TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
  /* Free the transport layer again. */
  TbxMbRtuFree(result);
} /*** end of test_TbxMbTpRtuCreate_CanCreate ***/


/************************************************************************************//**
** \brief     Tests that a RTU transport layer can be recreated, meaning that no new
**            memory is allocated from the heap via a memory pool.
**
****************************************************************************************/
void test_TbxMbTpRtuCreate_CanRecreate(void)
{
  tTbxMbTp result;
  size_t   heapFreeBefore;
  size_t   heapFreeAfter;

  /* Create a RTU transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                          TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the transport layer. */
  assertionCnt = 0;
  TbxMbRtuFree(result);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbTpRtuCreate_CanRecreate ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbTpRtuFree_ShouldAssertOnInvalidParams(void)
{
  size_t heapFreeBefore;
  size_t heapFreeAfter;

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbRtuFree(NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a rtu context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbRtuFree(&invalidCtx);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbTpRtuFree_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a RTU transport layer can be freed.
**
****************************************************************************************/
void test_TbxMbTpRtuFree_CanFree(void)
{
  tTbxMbTp result;

  /* Create a RTU transport layer to test that it can be free. */
  assertionCnt = 0;
  result = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                          TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the transport layer. */
  assertionCnt = 0;
  TbxMbRtuFree(result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
} /*** end of test_TbxMbTpRtuFree_CanFree ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns NULL.
**
****************************************************************************************/
void test_TbxMbServerCreate_ShouldAssertOnInvalidParams(void)
{
  tTbxMbServer result;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbServerCreate(NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbServerCreate_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that trying to link an already used transport layer trigger an
**            assertion and returns NULL.
**
****************************************************************************************/
void test_TbxMbServerCreate_ShouldAssertOnUsedTp(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer1;
  tTbxMbServer mbServer2;

  /* Create the transport layer. */
  assertionCnt = 0;
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create first server that links the transport layer. */
  assertionCnt = 0;
  mbServer1 = TbxMbServerCreate(tpRtu);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbServer1);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create second server that tries the link the same transport layer. */
  assertionCnt = 0;
  mbServer2 = TbxMbServerCreate(tpRtu);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, mbServer2);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);

  /* Free the first server and transport layer. */
  assertionCnt = 0;
  TbxMbServerFree(mbServer1);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerCreate_ShouldAssertOnUsedTp ***/


/************************************************************************************//**
** \brief     Tests that a Modbus server can be created.
**
****************************************************************************************/
void test_TbxMbServerCreate_CanCreate(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  assertionCnt = 0;
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  assertionCnt = 0;
  mbServer = TbxMbServerCreate(tpRtu);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbServer);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport layer. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerCreate_CanCreate ***/


/************************************************************************************//**
** \brief     Tests that a Modbus server can be recreated, meaning that no new memory is
**            allocated from the heap via a memory pool.
**
****************************************************************************************/
void test_TbxMbServerCreate_CanRecreate(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  assertionCnt = 0;
  mbServer = TbxMbServerCreate(tpRtu);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbServer);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport layer. */
  assertionCnt = 0;
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbServerCreate_CanRecreate ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerFree_ShouldAssertOnInvalidParams(void)
{
  size_t heapFreeBefore;
  size_t heapFreeAfter;

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerFree(NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerFree(&invalidCtx);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbServerFree_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus server can be freed.
**
****************************************************************************************/
void test_TbxMbServerFree_CanFree(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  assertionCnt = 0;
  mbServer = TbxMbServerCreate(tpRtu);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbServer);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server. */
  assertionCnt = 0;
  TbxMbServerFree(mbServer);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the transport layer. */
  assertionCnt = 0;
  TbxMbRtuFree(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  heapFreeAfter = TbxHeapGetFree();
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbServerFree_CanFree ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadInput_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadInput(NULL, mbServer_ReadInput);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadInput(&invalidCtx, mbServer_ReadInput);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the callback function pointer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadInput(mbServer, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadInput_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the callback function can be set.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadInput_CanSet(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try setting the callback functioin. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadInput(mbServer, mbServer_ReadInput);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadInput_CanSet ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadCoil_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadCoil(NULL, mbServer_ReadCoil);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadCoil(&invalidCtx, mbServer_ReadCoil);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the callback function pointer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadCoil(mbServer, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadCoil_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the callback function can be set.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadCoil_CanSet(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try setting the callback functioin. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadCoil(mbServer, mbServer_ReadCoil);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadCoil_CanSet ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackWriteCoil_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackWriteCoil(NULL, mbServer_WriteCoil);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackWriteCoil(&invalidCtx, mbServer_WriteCoil);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the callback function pointer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackWriteCoil(mbServer, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackWriteCoil_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the callback function can be set.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackWriteCoil_CanSet(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try setting the callback functioin. */
  assertionCnt = 0;
  TbxMbServerSetCallbackWriteCoil(mbServer, mbServer_WriteCoil);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackWriteCoil_CanSet ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadInputReg_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadInputReg(NULL, mbServer_ReadInputReg);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadInputReg(&invalidCtx, mbServer_ReadInputReg);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the callback function pointer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadInputReg(mbServer, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadInputReg_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the callback function can be set.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadInputReg_CanSet(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try setting the callback functioin. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadInputReg(mbServer, mbServer_ReadInputReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadInputReg_CanSet ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadHoldingReg_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadHoldingReg(NULL, mbServer_ReadHoldingReg);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadHoldingReg(&invalidCtx, mbServer_ReadHoldingReg);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the callback function pointer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackReadHoldingReg(mbServer, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadHoldingReg_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the callback function can be set.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackReadHoldingReg_CanSet(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try setting the callback functioin. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadHoldingReg(mbServer, mbServer_ReadHoldingReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackReadHoldingReg_CanSet ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackWriteHoldingReg_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackWriteHoldingReg(NULL, mbServer_WriteHoldingReg);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackWriteHoldingReg(&invalidCtx, mbServer_WriteHoldingReg);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the callback function pointer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackWriteHoldingReg(mbServer, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackWriteHoldingReg_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the callback function can be set.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackWriteHoldingReg_CanSet(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try setting the callback functioin. */
  assertionCnt = 0;
  TbxMbServerSetCallbackWriteHoldingReg(mbServer, mbServer_WriteHoldingReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackWriteHoldingReg_CanSet ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackCustomFunction_ShouldAssertOnInvalidParams(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try NULL as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackCustomFunction(NULL, mbServer_ReportServerIdCallback);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackCustomFunction(&invalidCtx, mbServer_ReportServerIdCallback);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the callback function pointer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerSetCallbackCustomFunction(mbServer, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackCustomFunction_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that the callback function can be set.
**
****************************************************************************************/
void test_TbxMbServerSetCallbackCustomFunction_CanSet(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbServer mbServer;

  /* First create a transport protocol context and a server context. */
  tpRtu = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  TEST_ASSERT_NOT_NULL(tpRtu);
  mbServer = TbxMbServerCreate(tpRtu);
  TEST_ASSERT_NOT_NULL(tpRtu);

  /* Try setting the callback functioin. */
  assertionCnt = 0;
  TbxMbServerSetCallbackCustomFunction(mbServer, mbServer_ReportServerIdCallback);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the server and transport protocol. */
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbServerSetCallbackCustomFunction_CanSet ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns NULL.
**
****************************************************************************************/
void test_TbxMbClientCreate_ShouldAssertOnInvalidParams(void)
{
  tTbxMbClient result;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  result = TbxMbClientCreate(NULL, 1000U, 1000U);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbClientCreate_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that trying to link an already used transport layer trigger an
**            assertion and returns NULL.
**
****************************************************************************************/
void test_TbxMbClientCreate_ShouldAssertOnUsedTp(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient1;
  tTbxMbClient mbClient2;

  /* Create the transport layer. */
  assertionCnt = 0;
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create first client that links the transport layer. */
  assertionCnt = 0;
  mbClient1 = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient1);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create second client that tries the link the same transport layer. */
  assertionCnt = 0;
  mbClient2 = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(NULL, mbClient2);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);

  /* Free the first server and transport layer. */
  assertionCnt = 0;
  TbxMbClientFree(mbClient1);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientCreate_ShouldAssertOnUsedTp ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can be created.
**
****************************************************************************************/
void test_TbxMbClientCreate_CanCreate(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;

  assertionCnt = 0;
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientCreate_CanCreate ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can be recreated, meaning that no new memory is
**            allocated from the heap via a memory pool.
**
****************************************************************************************/
void test_TbxMbClientCreate_CanRecreate(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the client and transport layer. */
  assertionCnt = 0;
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbClientCreate_CanRecreate ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion.
**
****************************************************************************************/
void test_TbxMbClientFree_ShouldAssertOnInvalidParams(void)
{
  size_t heapFreeBefore;
  size_t heapFreeAfter;

  /* Try NULL as a client context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbClientFree(NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try passing a dummy context with an invalid type as a server context. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbClientFree(&invalidCtx);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbClientFree_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can be freed.
**
****************************************************************************************/
void test_TbxMbClientFree_CanFree(void)
{
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the client. */
  assertionCnt = 0;
  TbxMbClientFree(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the transport layer. */
  assertionCnt = 0;
  TbxMbRtuFree(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  heapFreeAfter = TbxHeapGetFree();
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);
} /*** end of test_TbxMbClientFree_CanFree ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientReadCoils_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;
  uint8_t      coils[2] = { TBX_OFF, TBX_OFF };

  /* Create a transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a client channel. */
  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientReadCoils(NULL, 10U, 0U, 2U, coils);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 0 as number of coils to read. */
  assertionCnt = 0;
  result = TbxMbClientReadCoils(mbClient, 10U, 0U, 0U, coils);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 2001 as number of coils to read. */
  assertionCnt = 0;
  result = TbxMbClientReadCoils(mbClient, 10U, 0U, 2001U, coils);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the coils data pointer. */
  assertionCnt = 0;
  result = TbxMbClientReadCoils(mbClient, 10U, 0U, 2U, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientReadCoils_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can read coils from a Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadCoils_CanRead(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint8_t coils[2] = { TBX_ON, TBX_ON };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadCoil(mbServer, mbServer_ReadCoil);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the two coils supported by the server. */
  assertionCnt = 0;
  result = TbxMbClientReadCoils(mbClient, 10U, 0U, 2U, coils);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read coils were as expected. */
  TEST_ASSERT_EQUAL_UINT8(TBX_OFF, coils[0]);
  TEST_ASSERT_EQUAL_UINT8(TBX_OFF, coils[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadCoils_CanRead ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client cannot read coils that are not supported by the
**            Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadCoils_CannotReadUnsupported(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint8_t coils[3] = { TBX_OFF, TBX_OFF, TBX_OFF };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadCoil(mbServer, mbServer_ReadCoil);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the three coils, while knowing that the third one is not supported by the 
   * server. 
   */
  assertionCnt = 0;
  result = TbxMbClientReadCoils(mbClient, 10U, 0U, 3U, coils);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Read coils that are not supported by Modbus server.  */
  assertionCnt = 0;
  result = TbxMbClientReadCoils(mbClient, 10U, 2U, 2U, coils);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadCoils_CannotReadUnsupported ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientReadInputs_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;
  uint8_t      inputs[2] = { TBX_OFF, TBX_OFF };

  /* Create a transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a client channel. */
  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(NULL, 10U, 10000U, 2U, inputs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 0 as number of inputs to read. */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(mbClient, 10U, 10000U, 0U, inputs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 2001 as number of inputs to read. */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(mbClient, 10U, 10000U, 2001U, inputs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the inputs data pointer. */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(mbClient, 10U, 10000U, 2U, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientReadInputs_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can read inputs from a Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadInputs_CanRead(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint8_t      inputs[2] = { TBX_OFF, TBX_ON };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadInput(mbServer, mbServer_ReadInput);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the two inputs supported by the server. */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(mbClient, 10U, 10000U, 2U, inputs);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read inputs were as expected. */
  TEST_ASSERT_EQUAL_UINT8(TBX_ON, inputs[0]);
  TEST_ASSERT_EQUAL_UINT8(TBX_OFF, inputs[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadInputs_CanRead ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client cannot read inputs that are not supported by the
**            Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadInputs_CannotReadUnsupported(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint8_t inputs[3] = { TBX_OFF, TBX_OFF, TBX_OFF };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadInput(mbServer, mbServer_ReadInput);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the three inputs, while knowing that the third one is not supported by the 
   * server. 
   */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(mbClient, 10U, 10000U, 3U, inputs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Read inputs that are not supported by Modbus server.  */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(mbClient, 10U, 10002U, 2U, inputs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Read inputs that are not supported by Modbus server.  */
  assertionCnt = 0;
  result = TbxMbClientReadInputs(mbClient, 10U, 9998U, 2U, inputs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadInputs_CannotReadUnsupported ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientReadInputRegs_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;
  uint16_t     inputRegs[2] = { 0U, 0U };

  /* Create a transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a client channel. */
  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(NULL, 10U, 30000U, 2U, inputRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 0 as number of input registers to read. */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(mbClient, 10U, 30000U, 0U, inputRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 126 as number of input registers to read. */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(mbClient, 10U, 30000U, 126U, inputRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the input registers' data pointer. */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(mbClient, 10U, 30000U, 2U, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientReadInputRegs_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can read input registers from a Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadInputRegs_CanRead(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     inputRegs[2] = { 0U, 0U };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadInputReg(mbServer, mbServer_ReadInputReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the two input registers supported by the server. */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(mbClient, 10U, 30000U, 2U, inputRegs);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read inputs were as expected. */
  TEST_ASSERT_EQUAL_UINT16(0x1234U, inputRegs[0]);
  TEST_ASSERT_EQUAL_UINT16(0x5678U, inputRegs[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadInputRegs_CanRead ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client cannot read input registers that are not
**            supported by the Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadInputRegs_CannotReadUnsupported(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     inputRegs[3] = { 0U, 0U, 0U };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadInputReg(mbServer, mbServer_ReadInputReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the three input registers, while knowing that the third one is not supported
   * by the server. 
   */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(mbClient, 10U, 30000U, 3U, inputRegs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Read input registers that are not supported by Modbus server.  */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(mbClient, 10U, 30002U, 2U, inputRegs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Read input registers that are not supported by Modbus server.  */
  assertionCnt = 0;
  result = TbxMbClientReadInputRegs(mbClient, 10U, 29998U, 2U, inputRegs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadInputRegs_CannotReadUnsupported ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientReadHoldingRegs_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;
  uint16_t     holdingRegs[2] = { 0U, 0U };

  /* Create a transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a client channel. */
  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(NULL, 10U, 40000U, 2U, holdingRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 0 as number of holding registers to read. */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40000U, 0U, holdingRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 126 as number of holding registers to read. */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40000U, 126U, holdingRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the holding registers' data pointer. */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40000U, 2U, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientReadHoldingRegs_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can read holding registers from a Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadHoldingRegs_CanRead(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     holdingRegs[2] = { 0U, 0U };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadHoldingReg(mbServer, mbServer_ReadHoldingReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the two holding registers supported by the server. */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40000U, 2U, holdingRegs);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read inputs were as expected. */
  TEST_ASSERT_EQUAL_UINT16(0x789AU, holdingRegs[0]);
  TEST_ASSERT_EQUAL_UINT16(0xA51FU, holdingRegs[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadHoldingRegs_CanRead ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client cannot read holding registers that are not
**            supported by the Modbus server.
**
****************************************************************************************/
void test_TbxMbClientReadHoldingRegs_CannotReadUnsupported(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     holdingRegs[3] = { 0U, 0U, 0U };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackReadHoldingReg(mbServer, mbServer_ReadHoldingReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Read the three holding registers, while knowing that the third one is not supported
   * by the server. 
   */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40000U, 3U, holdingRegs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Read holding registers that are not supported by Modbus server.  */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40002U, 2U, holdingRegs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Read holding registers that are not supported by Modbus server.  */
  assertionCnt = 0;
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 39998U, 2U, holdingRegs);
  /* Make sure the client operation reported an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientReadHoldingRegs_CannotReadUnsupported ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientWriteCoils_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;
  uint8_t      coils[2] = { TBX_OFF, TBX_OFF };

  /* Create a transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a client channel. */
  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientWriteCoils(NULL, 10U, 0U, 2U, coils);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 0 as number of coils to write. */
  assertionCnt = 0;
  result = TbxMbClientWriteCoils(mbClient, 10U, 0U, 0U, coils);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 1969 as number of coils to write. */
  assertionCnt = 0;
  result = TbxMbClientWriteCoils(mbClient, 10U, 0U, 1969U, coils);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the coils data pointer. */
  assertionCnt = 0;
  result = TbxMbClientWriteCoils(mbClient, 10U, 0U, 2U, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientWriteCoils_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can write coils to a Modbus server.
**
****************************************************************************************/
void test_TbxMbClientWriteCoils_CanWrite(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint8_t      coils[2] = { TBX_OFF, TBX_OFF };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackWriteCoil(mbServer, mbServer_WriteCoil);
  TbxMbServerSetCallbackReadCoil(mbServer, mbServer_ReadCoil);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Write the two coils supported by the server to ON and OFF. */
  assertionCnt = 0;
  coils[0] = TBX_ON;
  coils[1] = TBX_OFF;
  result = TbxMbClientWriteCoils(mbClient, 10U, 0U, 2U, coils);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  result = TbxMbClientReadCoils(mbClient, 10U, 0U, 2U, coils);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read coils were as expected. */
  TEST_ASSERT_EQUAL_UINT8(TBX_ON, coils[0]);
  TEST_ASSERT_EQUAL_UINT8(TBX_OFF, coils[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Write the two coils supported by the server to OFF and ON. */
  assertionCnt = 0;
  coils[0] = TBX_OFF;
  coils[1] = TBX_ON;
  result = TbxMbClientWriteCoils(mbClient, 10U, 0U, 2U, coils);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  result = TbxMbClientReadCoils(mbClient, 10U, 0U, 2U, coils);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read coils were as expected. */
  TEST_ASSERT_EQUAL_UINT8(TBX_OFF, coils[0]);
  TEST_ASSERT_EQUAL_UINT8(TBX_ON, coils[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientWriteCoils_CanWrite ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client cannot write coils that are not supported by the
**            Modbus server.
**
****************************************************************************************/
void test_TbxMbClientWriteCoils_CannotWriteUnsupported(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint8_t      coils[3] = { TBX_OFF, TBX_OFF, TBX_OFF };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackWriteCoil(mbServer, mbServer_WriteCoil);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Write the three coils, while knowing that the third one is not supported by the 
   * server.
   */
  assertionCnt = 0;
  result = TbxMbClientWriteCoils(mbClient, 10U, 0U, 3U, coils);
  /* Make sure the client operation repored an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Write the coils that are not supported by the server. */
  assertionCnt = 0;
  result = TbxMbClientWriteCoils(mbClient, 10U, 2U, 2U, coils);
  /* Make sure the client operation repored an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientWriteCoils_CannotWriteUnsupported ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientWriteHoldingRegs_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;
  uint16_t     holdingRegs[2] = { 0U, 0U };

  /* Create a transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a client channel. */
  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientWriteHoldingRegs(NULL, 10U, 40000U, 2U, holdingRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 0 as number of holding registers to write. */
  assertionCnt = 0;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40000U, 0U, holdingRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try 124 as number of holding registers to write. */
  assertionCnt = 0;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40000U, 124U, holdingRegs);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the holding registers data pointer. */
  assertionCnt = 0;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40000U, 2U, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientWriteHoldingRegs_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can write holding registers to a Modbus server.
**
****************************************************************************************/
void test_TbxMbClientWriteHoldingRegs_CanWrite(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     holdingRegs[2] = { 0U, 0U };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackWriteHoldingReg(mbServer, mbServer_WriteHoldingReg);
  TbxMbServerSetCallbackReadHoldingReg(mbServer, mbServer_ReadHoldingReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Write the two holding registers supported by the server. */
  assertionCnt = 0;
  holdingRegs[0] = 1023;
  holdingRegs[1] = 0xA5F1;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40000U, 2U, holdingRegs);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40000U, 2U, holdingRegs);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read holding registers were as expected. */
  TEST_ASSERT_EQUAL_UINT16(1023, holdingRegs[0]);
  TEST_ASSERT_EQUAL_UINT16(0xA5F1, holdingRegs[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Write the two holding registers supported by the server to zero values. */
  assertionCnt = 0;
  holdingRegs[0] = 0x0000;
  holdingRegs[1] = 0x0000;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40000U, 2U, holdingRegs);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  result = TbxMbClientReadHoldingRegs(mbClient, 10U, 40000U, 2U, holdingRegs);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read holding registers were as expected. */
  TEST_ASSERT_EQUAL_UINT16(0x0000, holdingRegs[0]);
  TEST_ASSERT_EQUAL_UINT16(0x0000, holdingRegs[1]);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientWriteHoldingRegs_CanWrite ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client cannot write holding registers that are not
**            supported by the Modbus server.
**
****************************************************************************************/
void test_TbxMbClientWriteHoldingRegs_CannotWriteUnsupported(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     holdingRegs[3] = { 0U, 0U, 0U };

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackWriteHoldingReg(mbServer, mbServer_WriteHoldingReg);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Write the three holding registers, while knowing that the third one is not supported
   * by the server.
   */
  assertionCnt = 0;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40000U, 3U, holdingRegs);
  /* Make sure the client operation repored an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Write the holding registers that are not supported by the server. */
  assertionCnt = 0;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40002U, 2U, holdingRegs);
  /* Make sure the client operation repored an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Write the holding registers that are not supported by the server. */
  assertionCnt = 0;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 39998U, 2U, holdingRegs);
  /* Make sure the client operation repored an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Write a value to the first holding register that is outside of the range supported
   * by the server. 
   */
  assertionCnt = 0;
  holdingRegs[0] = 1024;
  result = TbxMbClientWriteHoldingRegs(mbClient, 10U, 40000U, 1U, holdingRegs);
  /* Make sure the client operation repored an error. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientWriteHoldingRegs_CannotWriteUnsupported ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientCustomFunction_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtu;
  tTbxMbClient mbClient;
  size_t       heapFreeBefore;
  size_t       heapFreeAfter;
  uint8_t      response[TBX_MB_TP_PDU_MAX_LEN]; 
  uint8_t      request[1] = { 17U };
  uint8_t      len = 1U;  

  /* Create a transport layer. */
  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  tpRtu = TbxMbRtuCreate(0, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                         TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(tpRtu);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a client channel. */
  assertionCnt = 0;
  mbClient = TbxMbClientCreate(tpRtu, 1000U, 1000U);
  /* Make sure a valid context was returned. */
  TEST_ASSERT_NOT_NULL(mbClient);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientCustomFunction(NULL, 10U, request, response, &len);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the transmit PDU packet data. */
  assertionCnt = 0;
  result = TbxMbClientCustomFunction(mbClient, 10U, NULL, response, &len);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the reception PDU packet data. */
  assertionCnt = 0;
  result = TbxMbClientCustomFunction(mbClient, 10U, request, NULL, &len);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Try NULL as the packet length. */
  assertionCnt = 0;
  result = TbxMbClientCustomFunction(mbClient, 10U, request, response, NULL);
  heapFreeAfter = TbxHeapGetFree();
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
  /* Make sure no heap memory was allocated. */
  TEST_ASSERT_EQUAL(heapFreeBefore, heapFreeAfter);

  /* Free the client and transport layer. */
  TbxMbClientFree(mbClient);
  TbxMbRtuFree(tpRtu);
} /*** end of test_TbxMbClientCustomFunction_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can use a custom function to extend the
**            default supported function codes with the one for "Report Server ID" and
**            can execute it to obtain the server's ID.
**
****************************************************************************************/
void test_TbxMbClientCustomFunction_CanExecute(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     serverId = 0U;
  uint8_t      response[TBX_MB_TP_PDU_MAX_LEN]; 
  uint8_t      request[1] = { 17U };
  uint8_t      len = 1U;  

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Set the callback for the server. */
  assertionCnt = 0;
  TbxMbServerSetCallbackCustomFunction(mbServer, mbServer_ReportServerIdCallback);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);
 
  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the Report Server ID function code packet. */
  assertionCnt = 0;

  /* Transceive function code 17 - Report Server ID. */
  result = TbxMbClientCustomFunction(mbClient, 10U, request, response, &len);
  if (result == TBX_OK)
  {
    /* Response length as expected? */
    if (len == 5U)
    {
      /* Not an exception response and byte count correct? */
      if ((response[0] == 17U) && (response[1] == 3U))
      {
        /* Read out the received server ID. */
        serverId = TbxMbCommonExtractUInt16BE(&response[2]);
      }
    }
  }
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the read server ID was as expected. */
  TEST_ASSERT_EQUAL_UINT16(0x1234U, serverId);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientCustomFunction_CanExecute ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can use a custom function to extend the
**            default supported function codes with the one for "Report Server ID", yet
**            cannot execute it to obtain the server's ID, from a server that does not
**            support this function code.
**
****************************************************************************************/
void test_TbxMbClientCustomFunction_CannotExecuteUnsupported(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint8_t      response[TBX_MB_TP_PDU_MAX_LEN]; 
  uint8_t      request[1] = { 17U };
  uint8_t      len = 1U;  

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the Report Server ID function code packet. */
  assertionCnt = 0;

  /* Transceive function code 17 - Report Server ID. */
  result = TbxMbClientCustomFunction(mbClient, 10U, request, response, &len);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the response is en error packet with:
   * - data[0] = function code + 0x80 = 145
   * - data[1] = packet data[0] = illegal function error code = 1
   * - data len = 2 (including function code)
   */
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT8(145, response[0]);
  TEST_ASSERT_EQUAL_UINT8(1, response[1]);
  TEST_ASSERT_EQUAL_UINT8(2, len);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientCustomFunction_CannotExecuteUnsupported ***/


/************************************************************************************//**
** \brief     Tests that invalid parameters trigger an assertion and returns TBX_ERROR.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_ShouldAssertOnInvalidParams(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     count = 0U;  

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                               TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Try NULL as a client context. */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(NULL, 10U, TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT, &count);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);

  /* Try NULL for the counter parameter for those subcodes that need this parameter. Note
   * that this requires a server response, therefore a server is needed for this test.
   * This is also the reason why the heap before/after test is not include, because
   * memory pool data is actually allocated.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT, NULL);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);

  /* Try an unsupported diagnostics subcode. */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, 500U, &count);
  /* Make sure an error was returned. */
  TEST_ASSERT_EQUAL(TBX_ERROR, result);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_ShouldAssertOnInvalidParams ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can perform the requested diagnostics operation.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_CanQueryData(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the diagnostics request to query data. */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_QUERY_DATA, NULL);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_CanQueryData ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can perform the requested diagnostics operation.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_CanClearCounters(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     count = 0U;

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the diagnostics request to query data. This should bring the bus message count
   * to 1.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_QUERY_DATA, NULL);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Send the diagnostics request to get the bus message count. This should bring the bus
   * message count to 2 and therefore return a count of 2.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 2. */
  TEST_ASSERT_EQUAL_UINT16(2, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Send the diagnostics request to clear the counters. This should bring the bus
   * message count to 0.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_CLEAR_COUNTERS, NULL);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Send the diagnostics request to get the bus message count. This should bring the bus
   * message count to 1 and therefore return a count of 1.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 1. */
  TEST_ASSERT_EQUAL_UINT16(1, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_CanClearCounters ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can perform the requested diagnostics operation.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_CanReadBusMessageCount(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     count = 0U;

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the diagnostics request to get the bus message count. This should bring the bus
   * message count to 1 and therefore return a count of 1.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 1. */
  TEST_ASSERT_EQUAL_UINT16(1, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_CanReadBusMessageCount ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can perform the requested diagnostics operation.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_CanReadBusCommErrorCount(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     count = 0U;

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the diagnostics request to get the bus comminucation error count. This should
   * be 0 as no packets with an invalid CRC were sent.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_BUS_COMM_ERROR_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 0. */
  TEST_ASSERT_EQUAL_UINT16(0, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_CanReadBusCommErrorCount ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can perform the requested diagnostics operation.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_CanReadBusExceptionErrorCount(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     count = 0U;
  uint8_t      response[TBX_MB_TP_PDU_MAX_LEN]; 
  uint8_t      request[1] = { 17U };
  uint8_t      len = 1U;  

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the diagnostics request to get the bus exception error count. This should
   * be 0 as no exception responses were sent so far.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_BUS_EXCEPTION_ERROR_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 0. */
  TEST_ASSERT_EQUAL_UINT16(0, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Transceive function code 17 - Report Server ID, which is not supported and should
   * trigger an exception response.
   */
  result = TbxMbClientCustomFunction(mbClient, 10U, request, response, &len);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the response is en error packet with:
   * - data[0] = function code + 0x80 = 145
   * - data[1] = packet data[0] = illegal function error code = 1
   * - data len = 2 (including function code)
   */
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT8(145, response[0]);
  TEST_ASSERT_EQUAL_UINT8(1, response[1]);
  TEST_ASSERT_EQUAL_UINT8(2, len);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Send the diagnostics request to get the bus exception error count. This should now
   * be 1.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_BUS_EXCEPTION_ERROR_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 1. */
  TEST_ASSERT_EQUAL_UINT16(1, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_CanReadBusExceptionErrorCount ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can perform the requested diagnostics operation.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_CanReadServerMessageCount(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     count = 0U;

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the diagnostics request to get the server message count. This should bring the
   * server message count to 1 and therefore return a count of 1.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_SERVER_MESSAGE_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 1. */
  TEST_ASSERT_EQUAL_UINT16(1, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_CanReadServerMessageCount ***/


/************************************************************************************//**
** \brief     Tests that a Modbus client can perform the requested diagnostics operation.
**
****************************************************************************************/
void test_TbxMbClientDiagnostics_CanReadServerNoResponseCount(void)
{
  uint8_t      result;
  tTbxMbTp     tpRtuServer;
  tTbxMbTp     tpRtuClient;
  tTbxMbServer mbServer;
  tTbxMbClient mbClient;
  uint16_t     count = 0U;

  /* Create a Modbus RTU server on serial port 1. */
  assertionCnt = 0;
  tpRtuServer = TbxMbRtuCreate(10, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbServer = TbxMbServerCreate(tpRtuServer);
  TEST_ASSERT_NOT_NULL(tpRtuServer);
  TEST_ASSERT_NOT_NULL(mbServer);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Create a Modbus RTU client on serial port 2. */
  assertionCnt = 0;
  tpRtuClient = TbxMbRtuCreate(0, TBX_MB_UART_PORT2, TBX_MB_UART_19200BPS, 
                              TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);
  mbClient = TbxMbClientCreate(tpRtuClient, 1000U, 1000U);
  TEST_ASSERT_NOT_NULL(tpRtuClient);
  TEST_ASSERT_NOT_NULL(mbClient);
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Bring the Modbus stack to an operational state in the simulated environment. */
  startupModbusStack();

  /* Send the diagnostics request to get the server no response count. This should be 0.
   */
  assertionCnt = 0;
  result = TbxMbClientDiagnostics(mbClient, 10U, TBX_MB_DIAG_SC_SERVER_NO_RESPONSE_COUNT, &count);
  /* Make sure the client operation was successful. */
  TEST_ASSERT_EQUAL(TBX_OK, result);
  /* Make sure the message count is 0. */
  TEST_ASSERT_EQUAL_UINT16(0, count);
  /* Make sure no assertion was triggered. */
  TEST_ASSERT_EQUAL_UINT32(0, assertionCnt);

  /* Free the channels and transport layers. */
  TbxMbClientFree(mbClient);
  TbxMbServerFree(mbServer);
  TbxMbRtuFree(tpRtuClient);
  TbxMbRtuFree(tpRtuServer);
} /*** end of test_TbxMbClientDiagnostics_CanReadServerNoResponseCount ***/


/************************************************************************************//**
** \brief     Handles the running of the unit tests.
** \return    Test results.
**
****************************************************************************************/
int runTests(void)
{
  /* Inform the framework that unit testing is about to start. */
  UNITY_BEGIN();

  /* Tests that are generic. */
  RUN_TEST(test_TbxMbGeneric_VersionMacrosShouldBePresent);
  RUN_TEST(test_TbxMbGeneric_FunctionCodeMacrosShouldBePresent);
  RUN_TEST(test_TbxMbGeneric_ExceptionCodeMacrosShouldBePresent);
  RUN_TEST(test_TbxMbGeneric_DiagnosticSubCodeMacrosShouldBePresent);
  RUN_TEST(test_TbxMbGeneric_MiscellaneousMacrosShouldBePresent);
  RUN_TEST(test_TbxMbGeneric_TpNodeAddressMacrosShouldBePresent);
  RUN_TEST(test_TbxMbGeneric_TpPduMacrosShouldBePresent);
  RUN_TEST(test_TbxMbGeneric_TypesShouldBePresent);
  /* Tests for the Modbus common API. */
  RUN_TEST(test_TbxMbCommonExtractUInt16BE_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbCommonExtractUInt16BE_CanExtract);
  RUN_TEST(test_TbxMbCommonStoreUInt16BE_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbCommonStoreUInt16BE_CanStore);
  /* Tests for the Modbus event API. */
  RUN_TEST(test_TbxMbEventTask_CanCall);
  /* Tests for the Modbus UART API. */
  RUN_TEST(test_TbxMbUartTransmitComplete_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbUartDataReceived_ShouldAssertOnInvalidParams);
  /* Tests for a Modbus RTU transport layer API. */
  RUN_TEST(test_TbxMbTpRtuCreate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbTpRtuCreate_CanCreate);
  RUN_TEST(test_TbxMbTpRtuCreate_CanRecreate);
  RUN_TEST(test_TbxMbTpRtuFree_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbTpRtuFree_CanFree);
  /* Tests for a Modbus server API. */
  RUN_TEST(test_TbxMbServerCreate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerCreate_ShouldAssertOnUsedTp);
  RUN_TEST(test_TbxMbServerCreate_CanCreate);
  RUN_TEST(test_TbxMbServerCreate_CanRecreate);
  RUN_TEST(test_TbxMbServerFree_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerFree_CanFree);
  RUN_TEST(test_TbxMbServerSetCallbackReadInput_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerSetCallbackReadInput_CanSet);
  RUN_TEST(test_TbxMbServerSetCallbackReadCoil_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerSetCallbackReadCoil_CanSet);
  RUN_TEST(test_TbxMbServerSetCallbackWriteCoil_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerSetCallbackWriteCoil_CanSet);
  RUN_TEST(test_TbxMbServerSetCallbackReadInputReg_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerSetCallbackReadInputReg_CanSet);
  RUN_TEST(test_TbxMbServerSetCallbackReadHoldingReg_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerSetCallbackReadHoldingReg_CanSet);
  RUN_TEST(test_TbxMbServerSetCallbackWriteHoldingReg_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerSetCallbackWriteHoldingReg_CanSet);
  RUN_TEST(test_TbxMbServerSetCallbackCustomFunction_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerSetCallbackCustomFunction_CanSet);
  /* Tests for the Modbus client API. Note that these also perform additional run-time
   * tests with an RTU server.
   */
  RUN_TEST(test_TbxMbClientCreate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientCreate_ShouldAssertOnUsedTp);
  RUN_TEST(test_TbxMbClientCreate_CanCreate);
  RUN_TEST(test_TbxMbClientCreate_CanRecreate);
  RUN_TEST(test_TbxMbClientFree_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientFree_CanFree);
  RUN_TEST(test_TbxMbClientReadCoils_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientReadCoils_CanRead);
  RUN_TEST(test_TbxMbClientReadCoils_CannotReadUnsupported);
  RUN_TEST(test_TbxMbClientReadInputs_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientReadInputs_CanRead);
  RUN_TEST(test_TbxMbClientReadInputs_CannotReadUnsupported);
  RUN_TEST(test_TbxMbClientReadInputRegs_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientReadInputRegs_CanRead);
  RUN_TEST(test_TbxMbClientReadInputRegs_CannotReadUnsupported);
  RUN_TEST(test_TbxMbClientReadHoldingRegs_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientReadHoldingRegs_CanRead);
  RUN_TEST(test_TbxMbClientReadHoldingRegs_CannotReadUnsupported);
  RUN_TEST(test_TbxMbClientWriteCoils_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientWriteCoils_CanWrite);
  RUN_TEST(test_TbxMbClientWriteCoils_CannotWriteUnsupported);
  RUN_TEST(test_TbxMbClientWriteHoldingRegs_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientWriteHoldingRegs_CanWrite);
  RUN_TEST(test_TbxMbClientWriteHoldingRegs_CannotWriteUnsupported);
  RUN_TEST(test_TbxMbClientCustomFunction_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientCustomFunction_CanExecute);
  RUN_TEST(test_TbxMbClientCustomFunction_CannotExecuteUnsupported);
  RUN_TEST(test_TbxMbClientDiagnostics_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbClientDiagnostics_CanQueryData);
  RUN_TEST(test_TbxMbClientDiagnostics_CanClearCounters);
  RUN_TEST(test_TbxMbClientDiagnostics_CanReadBusMessageCount);
  RUN_TEST(test_TbxMbClientDiagnostics_CanReadBusCommErrorCount);
  RUN_TEST(test_TbxMbClientDiagnostics_CanReadBusExceptionErrorCount);
  RUN_TEST(test_TbxMbClientDiagnostics_CanReadServerMessageCount);
  RUN_TEST(test_TbxMbClientDiagnostics_CanReadServerNoResponseCount);

  /* Inform the framework that unit testing is done and return the result. */
  return UNITY_END();
} /*** end of runUnittests ***/


/************************************************************************************//**
** \brief     Initialization before running the unit tests.
**
****************************************************************************************/
void initializeTests(void)
{
/* Register the application specific assertion handler. */
  TbxAssertSetHandler(handleTbxAssertion);
} /*** end of initializeTests ***/


/************************************************************************************//**
** \brief     Code to run before each test.
**
****************************************************************************************/
void setUp(void)
{
  /* Reset the assertion counter. */
  assertionCnt = 0;
} /*** end of setUp ***/


/************************************************************************************//**
** \brief     Code to run after each test.
**
****************************************************************************************/
void tearDown(void)
{
} /*** end of tearDown ***/


/*********************************** end of unittests.c ********************************/
