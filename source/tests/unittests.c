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
#include <sys/time.h>                            /* Time definitions                   */


/****************************************************************************************
* Local data declarations
****************************************************************************************/
/** \brief Keeps track of how often an assertion got triggered. */
uint32_t assertionCnt = 0;

/** \brief Modbus server coils. */
uint8_t mbServerCoils[2] = { TBX_OFF, TBX_OFF };

/** \brief Modbus server holding registers. */
uint16_t mbServerHoldingRegs[2] = { 0U, 0U };


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
static tTbxMbServerResult mbServer_ReadCoil(tTbxMbServer channel, uint16_t addr,
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
static tTbxMbServerResult mbServer_WriteCoil(tTbxMbServer channel, uint16_t addr,
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
} /*** end of test_TbxMbTpRtuCreate_CanCreate ***/


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

  assertionCnt = 0;
  heapFreeBefore = TbxHeapGetFree();
  TbxMbServerFree(NULL);
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
  /* Tests for a Modbus RTU transport layer. */
  RUN_TEST(test_TbxMbTpRtuCreate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbTpRtuCreate_CanCreate);
  RUN_TEST(test_TbxMbTpRtuCreate_CanRecreate);
  RUN_TEST(test_TbxMbTpRtuFree_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbTpRtuFree_CanFree);
  /* Tests for a Modbus server. */
  RUN_TEST(test_TbxMbServerCreate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerCreate_CanCreate);
  RUN_TEST(test_TbxMbServerCreate_CanRecreate);
  RUN_TEST(test_TbxMbServerFree_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbServerFree_CanFree);
  /* TODO ##Vg CONTINUE HERE.
   *           Test XxxShouldAssertOnInvalidParams and XxxCanSetCallback for all the
   *           TbxMbServerSetCallbackXxx() API functions of the server. Note that 
   *           callback functions were already added.
   */

  /* TODO ##Vg Tests for a Modbus client. Note that these also perform addition test
   *           with a server, otherwise the client API cannot be tested.
   */

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
