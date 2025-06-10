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
static tTbxMbServerResult mbServer_ReadInputReg(tTbxMbServer channel, uint16_t addr, 
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
** \brief     Tests that invalid parameters trigger an assertion and returns NULL.
**
****************************************************************************************/
void test_TbxMbTpRtuFree_ShouldAssertOnInvalidParams(void)
{
  assertionCnt = 0;
  TbxMbRtuFree(NULL);
  /* Make sure an assertion was triggered. */
  TEST_ASSERT_GREATER_THAN_UINT32(0, assertionCnt);
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
  /* Test for a Modbus RTU transport layer. */
  RUN_TEST(test_TbxMbTpRtuCreate_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbTpRtuCreate_CanCreate);
  RUN_TEST(test_TbxMbTpRtuCreate_CanRecreate);
  RUN_TEST(test_TbxMbTpRtuFree_ShouldAssertOnInvalidParams);
  RUN_TEST(test_TbxMbTpRtuFree_CanFree);

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
