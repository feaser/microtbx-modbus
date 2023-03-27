# API reference

This section provides a full reference of all the functions, macros and types that MicroTBX-Modbus offers.

## Macros

### Version

| Macro                  | Description                              |
| :--------------------- | :--------------------------------------- |
| `TBX_MB_VERSION_MAIN`  | Main version number of MicroTBX-Modbus.  |
| `TBX_MB_VERSION_MINOR` | Minor version number of MicroTBX-Modbus. |
| `TBX_MB_VERSION_PATCH` | Patch number of MicroTBX-Modbus.         |

### Common

Function codes.

| Macro                                  | Description                                         |
| :------------------------------------- | :-------------------------------------------------- |
| `TBX_MB_FC01_READ_COILS`               | Modbus function code 01 - Read Coils.               |
| `TBX_MB_FC02_READ_DISCRETE_INPUTS`     | Modbus function code 02 - Read Discrete Inputs.     |
| `TBX_MB_FC03_READ_HOLDING_REGISTERS`   | Modbus function code 03 - Read Holding Registers.   |
| `TBX_MB_FC04_READ_INPUT_REGISTERS`     | Modbus function code 04 - Read Input Registers.     |
| `TBX_MB_FC05_WRITE_SINGLE_COIL`        | Modbus function code 05 - Write Single Coil.        |
| `TBX_MB_FC06_WRITE_SINGLE_REGISTER`    | Modbus function code 06 - Write Single Register.    |
| `TBX_MB_FC08_DIAGNOSTICS`              | Modbus function code 08 - Diagnostics.              |
| `TBX_MB_FC15_WRITE_MULTIPLE_COILS`     | Modbus function code 15 - Write Multiple Coils.     |
| `TBX_MB_FC16_WRITE_MULTIPLE_REGISTERS` | Modbus function code 16 - Write Multiple Registers. |

Exception codes.

| Macro                               | Description                                       |
| :---------------------------------- | :------------------------------------------------ |
| `TBX_MB_EC01_ILLEGAL_FUNCTION`      | Modbus exception code 01 - Illegal function.      |
| `TBX_MB_EC02_ILLEGAL_DATA_ADDRESS`  | Modbus exception code 02 - Illegal data address.  |
| `TBX_MB_EC03_ILLEGAL_DATA_VALUE`    | Modbus exception code 03 - Illegal data value.    |
| `TBX_MB_EC04_SERVER_DEVICE_FAILURE` | Modbus exception code 04 - Server device failure. |

Diagnostics sub function codes.

| Macro                                      | Description                                                  |
| :----------------------------------------- | :----------------------------------------------------------- |
| `TBX_MB_DIAG_SC_QUERY_DATA`                | Diagnostics sub-function code - Return Query Data.           |
| `TBX_MB_DIAG_SC_CLEAR_COUNTERS`            | Diagnostics sub-function code - Clear Counters.              |
| `TBX_MB_DIAG_SC_BUS_MESSAGE_COUNT`         | Diagnostics sub-function code - Return Bus Message<br>Count. |
| `TBX_MB_DIAG_SC_BUS_COMM_ERROR_COUNT`      | Diagnostics sub-function code - Return Bus<br>Communication Error Count. |
| `TBX_MB_DIAG_SC_BUS_EXCEPTION_ERROR_COUNT` | Diagnostics sub-function code - Return Bus Exception<br>Error Count. |
| `TBX_MB_DIAG_SC_SERVER_MESSAGE_COUNT`      | Diagnostics sub-function code - Return Server Message<br>Count. |
| `TBX_MB_DIAG_SC_SERVER_NO_RESPONSE_COUNT`  | Diagnostics sub-function code - Return Server No<br>Response Count. |

Miscellaneous.

| Macro                      | Description                                                  |
| :------------------------- | :----------------------------------------------------------- |
| `TBX_MB_FC_EXCEPTION_MASK` | Bit mask to OR to the function code to flag it as an exception response. |

### Transport layer

Node address.

| Macro                           | Description                                |
| :------------------------------ | :----------------------------------------- |
| `TBX_MB_TP_NODE_ADDR_BROADCAST` | Node address value for broadcast purposes. |
| `TBX_MB_TP_NODE_ADDR_MIN`       | Minimum value of a valid node address.     |
| `TBX_MB_TP_NODE_ADDR_MAX`       | Maximum value of a valid node address.     |

Protocol data unit (PDU).

| Macro                        | Description                                                  |
| :--------------------------- | :----------------------------------------------------------- |
| `TBX_MB_TP_PDU_CODE_LEN_MAX` | Maximum size of the "Function code" at the start of a PDU.   |
| `TBX_MB_TP_PDU_DATA_LEN_MAX` | Maximum number of data bytes inside a PDU. This excludes the<br>function code. |
| `TBX_MB_TP_PDU_MAX_LEN`      | Maximum length of a PDU.                                     |

## Types

### Server

#### tTbxMbServer

```c
typedef void * tTbxMbServer
```

Handle to a Modbus server channel object, in the format of an opaque pointer.

#### tTbxMbServerResult

```c
typedef enum
{
  TBX_MB_SERVER_OK = 0U,
  TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR,
  TBX_MB_SERVER_ERR_DEVICE_FAILURE
} tTbxMbServerResult
```

numerated type with all supported return values for the callbacks.

#### tTbxMbServerReadInput

```c
typedef tTbxMbServerResult (* tTbxMbServerReadInput)(tTbxMbServer   channel, 
                                                     uint16_t       addr, 
                                                     uint8_t      * value)
```

Modbus server callback function for reading a discrete input.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `channel` | Handle to the Modbus server channel object that triggered the callback. |
| `addr`    | Element address (`0`..`65535`).                              |
| `value`   | Pointer to write the value of the input to. Use `TBX_ON` if the input is on, `TBX_OFF`<br>otherwise. |

| Return value                                                 |
| ------------------------------------------------------------ |
| `TBX_MB_SERVER_OK` if successful, `TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR` if the specific data element<br>address is not supported by this server, `TBX_MB_SERVER_ERR_DEVICE_FAILURE` otherwise. |

#### tTbxMbServerReadCoil

```c
typedef tTbxMbServerResult (* tTbxMbServerReadCoil)(tTbxMbServer   channel, 
                                                    uint16_t       addr, 
                                                    uint8_t      * value)
```

Modbus server callback function for reading a coil.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `channel` | Handle to the Modbus server channel object that triggered the callback. |
| `addr`    | Element address (`0`..`65535`).                              |
| `value`   | Pointer to write the value of the coil to. Use `TBX_ON` if the coils is on, `TBX_OFF`<br>otherwise. |

| Return value                                                 |
| ------------------------------------------------------------ |
| `TBX_MB_SERVER_OK` if successful, `TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR` if the specific data element<br>address is not supported by this server, `TBX_MB_SERVER_ERR_DEVICE_FAILURE` otherwise. |

#### tTbxMbServerWriteCoil

```c
typedef tTbxMbServerResult (* tTbxMbServerWriteCoil)(tTbxMbServer channel, 
                                                     uint16_t     addr, 
                                                     uint8_t      value)
```

Modbus server callback function for writing a coil.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `channel` | Handle to the Modbus server channel object that triggered the callback. |
| `addr`    | Element address (`0`..`65535`).                              |
| `value`   | Coil value. Use `TBX_ON` to activate the coil, `TBX_OFF` otherwise. |

| Return value                                                 |
| ------------------------------------------------------------ |
| `TBX_MB_SERVER_OK` if successful, `TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR` if the specific data element<br>address is not supported by this server, `TBX_MB_SERVER_ERR_DEVICE_FAILURE` otherwise. |

#### tTbxMbServerReadInputReg

```c
typedef tTbxMbServerResult (* tTbxMbServerReadInputReg)(tTbxMbServer   channel, 
                                                        uint16_t       addr, 
                                                        uint16_t     * value)
```

Modbus server callback function for reading an input register.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `channel` | Handle to the Modbus server channel object that triggered the callback. |
| `addr`    | Element address (`0`..`65535`).                              |
| `value`   | Pointer to write the value of the input register to.         |

| Return value                                                 |
| ------------------------------------------------------------ |
| `TBX_MB_SERVER_OK` if successful, `TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR` if the specific data element<br>address is not supported by this server, `TBX_MB_SERVER_ERR_DEVICE_FAILURE` otherwise. |

#### tTbxMbServerReadHoldingReg

```c
typedef tTbxMbServerResult (* tTbxMbServerReadHoldingReg)(tTbxMbServer   channel, 
                                                          uint16_t       addr, 
                                                          uint16_t     * value)
```

Modbus server callback function for reading a holding register.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `channel` | Handle to the Modbus server channel object that triggered the callback. |
| `addr`    | Element address (`0`..`65535`).                              |
| `value`   | Pointer to write the value of the holding register to.       |

| Return value                                                 |
| ------------------------------------------------------------ |
| `TBX_MB_SERVER_OK` if successful, `TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR` if the specific data element<br>address is not supported by this server, `TBX_MB_SERVER_ERR_DEVICE_FAILURE` otherwise. |

#### tTbxMbServerWriteHoldingReg

```c
typedef tTbxMbServerResult (* tTbxMbServerWriteHoldingReg)(tTbxMbServer channel, 
                                                           uint16_t     addr, 
                                                           uint16_t     value)
```

Modbus server callback function for writing a holding register.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `channel` | Handle to the Modbus server channel object that triggered the callback. |
| `addr`    | Element address (`0`..`65535`).                              |
| `value`   | Value of the holding register.                               |

| Return value                                                 |
| ------------------------------------------------------------ |
| `TBX_MB_SERVER_OK` if successful, `TBX_MB_SERVER_ERR_ILLEGAL_DATA_ADDR` if the specific data element<br>address is not supported by this server, `TBX_MB_SERVER_ERR_DEVICE_FAILURE` otherwise. |

#### tTbxMbServerCustomFunction

```c
typedef uint8_t (* tTbxMbServerCustomFunction)(tTbxMbServer    channel,
                                               uint8_t const * rxPdu,
                                               uint8_t       * txPdu,
                                               uint8_t       * len)
```

Modbus server callback function for implementing custom function code handling. Thanks to this functionality, the user can support Modbus function codes that are either currently not supported or user defined extensions.

The `rxPdu` and `txPdu` parameters are pointers to the byte array of the PDU. The first byte (i.e. `rxPdu[0]`) contains the function code, followed by its data bytes. Upon calling the callback, the `len` parameter contains the length of `rxPdu`. When preparing the response, you can write the length of the `txPdu` response to `len` as well.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| `channel` | Handle to the Modbus server channel object that triggered the callback. |
| `rxPdu`   | Pointer to a byte array for reading the received PDU.        |
| `txPdu`   | Pointer to a byte array for writing the response PDU.        |
| `len`     | Pointer to the PDU length, including the function code.      |

| Return value                                                 |
| ------------------------------------------------------------ |
| `TBX_TRUE` if the callback function handled the received function code and prepared a response PDU.<br>`TBX_FALSE` otherwise. |

### Client

#### tTbxMbClient

```c
typedef void * tTbxMbClient
```

Handle to a Modbus client channel object, in the format of an opaque pointer.

### Transport layer

#### tTbxMbTp

```c
typedef void * tTbxMbTp
```

Handle to a Modbus transport layer object, in the format of an opaque pointer.

### UART

#### tTbxMbUartPort

```c
typedef enum
{
  TBX_MB_UART_PORT1 = 0U,
  TBX_MB_UART_PORT2,
  TBX_MB_UART_PORT3,
  TBX_MB_UART_PORT4,
  TBX_MB_UART_PORT5,
  TBX_MB_UART_PORT6,
  TBX_MB_UART_PORT7,
  TBX_MB_UART_PORT8,
  TBX_MB_UART_NUM_PORT
} tTbxMbUartPort
```

Enumerated type with all supported UART ports.

#### tTbxMbUartBaudrate

```c
typedef enum
{
  TBX_MB_UART_1200BPS = 0U,
  TBX_MB_UART_2400BPS,
  TBX_MB_UART_4800BPS,
  TBX_MB_UART_9600BPS,
  TBX_MB_UART_19200BPS,
  TBX_MB_UART_38400BPS,
  TBX_MB_UART_57600BPS,
  TBX_MB_UART_115200BPS,
  TBX_MB_UART_NUM_BAUDRATE
} tTbxMbUartBaudrate
```

Enumerated type with all supported UART baudrates.

#### tTbxMbUartDatabits

```c
typedef enum
{
  TBX_MB_UART_7_DATABITS = 0U,
  TBX_MB_UART_8_DATABITS,
  TBX_MB_UART_NUM_DATABITS
} tTbxMbUartDatabits
```

Enumerated type with all supported UART data bits modes.

#### tTbxMbUartStopbits

```c
typedef enum
{
  TBX_MB_UART_1_STOPBITS = 0U,
  TBX_MB_UART_2_STOPBITS,
  TBX_MB_UART_NUM_STOPBITS
} tTbxMbUartStopbits
```

Enumerated type with all supported parity modes.

#### tTbxMbUartParity

```c
typedef enum
{
  TBX_MB_ODD_PARITY = 0U,
  TBX_MB_EVEN_PARITY,
  TBX_MB_NO_PARITY,
  TBX_MB_UART_NUM_PARITY
} tTbxMbUartParity
```

Enumerated type with all supported parity modes.

## Functions

### Server

<u>**TODO**</u>

### Client

**<u>TODO</u>**

### Event

#### TbxMbEventTask

```c
void TbxMbEventTask(void)
```

Task function that drives the entire Modbus stack. It processes internally generated events. How to call this function depends on the selected operating system abstraction layer (OSAL), which you determine based on the `source/osal/tbxmb_XXX.c` source file you compile and link with your firmware.

In a traditional superloop application (`tbxmb_superloop.c`), call this function continuously in the infinite program loop:

```c
#include <microtbx.h>
#include <microtbxmodbus.h>

void main(void)
{
  /* TODO Initialize the clock, enable peripherals and configure GPIO pins. */
  /* TODO Construct a Modbus transport layer object. */
  /* TODO Construct a Modbus client or server object. */

  /* Enter the program's infinite loop. */  
  for(;;)
  {
    /* Continuously call the Modbus stack event task function. */
    TbxMbEventTask();    
  } 
}
```

When using an RTOS (e.g. `tbxmb_freertos.c`), create a new task during application initialization and call this function from this task's infinite loop:

```c
#include <microtbx.h>
#include <microtbxmodbus.h>
#include "FreeRTOS.h"
#include "task.h"

void AppModbusTask(void * pvParameters);

void main(void)
{
  /* TODO Initialize the clock, enable peripherals and configure GPIO pins. */
  /* TODO Construct a Modbus transport layer object. */
  /* TODO Construct a Modbus client or server object. */

    /* Create the Modbus task. */
  xTaskCreate(AppModbusTask, "ModbusTask", configMINIMAL_STACK_SIZE, NULL, 4U, NULL);    
  /* Start the RTOS scheduler. Note that this function does not return. */
  vTaskStartScheduler();
}

void AppModbusTask(void * pvParameters)
{
  /* Enter infinite task loop. */
  for (;;)
  {
    /* Continuously call the Modbus stack event task function. */
    TbxMbEventTask();
  }
}
```

### RTU

#### TbxMbRtuCreate

```c
tTbxMbTp TbxMbRtuCreate(uint8_t            nodeAddr, 
                        tTbxMbUartPort     port, 
                        tTbxMbUartBaudrate baudrate,
                        tTbxMbUartStopbits stopbits,
                        tTbxMbUartParity   parity)
```

Creates a Modbus RTU transport layer object, which can later on be linked to a Modbus client or server channel.

Example for the following communication settings:

* First serial port on the board.
* Baudrate 19200 bits/second.
* 8 data-bits (default and fixed for an RTU transport layer).
* even parity.
* 1 stop-bit.
* Node address 10.

```c
tTbxMbTp modbusTp = TbxMbRtuCreate(10U, TBX_MB_UART_PORT1, TBX_MB_UART_19200BPS,
                                   TBX_MB_UART_1_STOPBITS, TBX_MB_EVEN_PARITY);   
```

| Parameter  | Description                                                  |
| ---------- | ------------------------------------------------------------ |
| `nodeAddr` | The address of the node. Can be in the range `1`..`247` for a server node. Set it to `0` for<br>a client. |
| `port`     | The serial port to use. The actual meaning of the serial port is hardware dependent. It<br>typically maps to the UART peripheral number. E.g. `TBX_MB_UART_PORT1` = USART1 on<br>an STM32. |
| `baudrate` | The desired communication speed.                             |
| `stopbits` | Number of stop bits at the end of a character.               |
| `parity`   | Parity bit type to use.                                      |

| Return value                                                 |
| ------------------------------------------------------------ |
| Handle to the newly created RTU transport layer object if successful, `NULL` otherwise. |

### UART

#### TbxMbUartTransmitComplete

```c
void TbxMbUartTransmitComplete(tTbxMbUartPort port)
```

Event function to signal to the UART module that the entire transfer, initiated by `TbxMbPortUartTransmit`, completed. This function should be called by the hardware specific UART port (located in `tbxmb_port.c`) at TX interrupt level.

| Parameter | Description                                     |
| --------- | ----------------------------------------------- |
| `port`    | The serial port that the transfer completed on. |

#### TbxMbUartDataReceived

```c
void TbxMbUartDataReceived(tTbxMbUartPort         port, 
                           uint8_t        const * data, 
                           uint8_t                len)
```

Event function to signal the reception of new data to the UART module. This function should be called by the hardware specific UART port (located in `tbxmb_port.c`) at RX interrupt level.

| Parameter | Description                                     |
| --------- | ----------------------------------------------- |
| `port`    | The serial port that the transfer completed on. |
| `data`    | Byte array with newly received data.            |
| `len`     | Number of newly received bytes.                 |
