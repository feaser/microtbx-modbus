# Create interface library for MicroTBX-Modbus sources.
add_library(microtbx-modbus INTERFACE)

target_sources(microtbx-modbus INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_uart.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_rtu.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_event.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_server.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_client.c"
)

target_include_directories(microtbx-modbus INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source"
)

# Create interface library for MicroTBX-Modbus OSAL superloop sources.
add_library(microtbx-modbus-osal-superloop INTERFACE)

target_sources(microtbx-modbus-osal-superloop INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/osal/tbxmb_superloop.c"
)

# Create interface library for MicroTBX-Modbus OSAL FreeRTOS sources.
add_library(microtbx-modbus-osal-freertos INTERFACE)

target_sources(microtbx-modbus-osal-freertos INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/osal/tbxmb_freertos.c"
)

# Create interface library for C++ extra sources.
add_library(microtbx-modbus-extra-cpp INTERFACE)

target_sources(microtbx-modbus-extra-cpp INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/cplusplus/tbxmbserver.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/cplusplus/tbxmbclient.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/cplusplus/tbxmbport.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/cplusplus/tbxmbevent.cpp"
)

target_include_directories(microtbx-modbus-extra-cpp INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/extra/cplusplus"
)

# Create interface library for the unit test specific sources.
add_library(microtbx-modbus-tests INTERFACE)

target_sources(microtbx-modbus-tests INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/tests/unittests.c"
)

target_include_directories(microtbx-modbus-tests INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/tests"
)

# Create interface library for the template. Only used for MISRA check.
add_library(microtbx-modbus-template INTERFACE)

target_include_directories(microtbx-modbus-template INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/template"
)

target_sources(microtbx-modbus-template INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/template/tbxmb_port.c"
)

