# Configuration

MicroTBX-Modbus was designed with ease of integration, portation and use in mind. As such, the default configuration works right out-of-the-box for the majority of use cases. In the rare situation that something doesn't work right, a few configuration macros exists to reconfigure the software component to your needs.

To change a value of a configuration macro, add the macro with the new value to the MicroTBX configuration header file `tbx_conf.h` in your application.

## Heap size

The most likely problem is that the MicroTBX heap size is configured too small. You typically notice this during initialization, when creating a channel object (e.g. `TbxMbServerCreate()`) or transport layer object (e.g. `TbxMbRtuCreate()`). The return value will be invalid and, if enabled, an assertion error is triggered. 

If this happens, increase the maximum heap size with macro `TBX_CONF_HEAP_SIZE`:

```c
/* Configure the size of the heap in bytes. */
#define TBX_CONF_HEAP_SIZE                       (4096U)
```

Usually when you hear that a software component uses the heap, alarm bells start going off. The general consensus is that one should not use the heap and only do static memory allocations. This is to prevent RAM memory fragmentation issues, possibly leading to an out-of-memory problem. 

However, MicroTBX-Modbus uses its own memory pools for dynamic allocation in a preallocated static RAM buffer that MicroTBX refers to as the heap. Its memory pool usage is such that there is no risk for memory fragmentation or out-of-memory problems. In fact, that's the entire idea behind the [memory pools](https://feaser.github.io/microtbx/mempools/) offered by MicroTBX: It enables you to do dynamic memory allocation on an embedded system in a safe manner, where it would otherwise not be possible or advisable.

## 1.5 character timeout detection

The Modbus RTU protocol requires that all bytes of a communication packet are communicated as a consecutive and uninterrupted data stream. The time between two bytes cannot be more than the time it would take to send 1.5 bytes (characters). The receiver of a Modbus RTU packet needs to continuously check for this 1.5 character timeout. When the received detect such a timeout, it should flag the Modbus RTU packet as invalid and not process it any further.

As opposed to most other available Modbus software stacks, MicroTBX-Modbus actually fully supports this 1.5 character timeout detection, when receiving Modbus RTU packets. However, it disables this feature by default for the following reasons: 

* Some Modbus RTU devices on the market do not adhere to this requirement.
* PCs, embedded Linux devices, and USB-RS232/RS485 adapters cannot always meet this requirement.

In case you know that all devices on your Modbus RTU network adhere to this requirements or you need to pass Modbus compliance testing, you can enable the 1.5 character timeout detection with the help of macro `TBX_MB_RTU_T1_5_TIMEOUT_ENABLE`:

```c
/* Enable the Modbus RTU 1.5 character timeout detection. */
#define TBX_MB_RTU_T1_5_TIMEOUT_ENABLE           (1U)
```

## Event queue size

To keep interrupt latency times as low as possible, MicroTBX-Modbus does as much processing as possible in its event task `TbxMbEventTask()` and not at interrupt level. Internally, events are posted to an event queue and they are consumed by the event task. At a given point in time the event queue can hold a (finite) amount of pending events. The macro `TBX_MB_EVENT_QUEUE_SIZE` configures its size. 

Only one scenario exists,where you would want to change the value of this macro: On a RAM constrained microcontroller, where you run out of RAM. In this case you want to set the event queue size as small as possible. This is basically 4 times (`TBX_MB_EVENT_NUM_ID`) the number of Modbus server and client channels that you create in your application:

```c
/* Configure the internal event queue size. Set it to 4 times the number of used
 * Modbus server and client channels that your application creates.
 */
#define TBX_MB_EVENT_QUEUE_SIZE                 (4U * 1U)
```

