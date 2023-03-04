# Create interface library for MicroTBX-Modbus sources.
add_library(microtbx-modbus INTERFACE)

target_sources(microtbx-modbus INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_uart.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_rtu.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_osal.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_event.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_server.c"
    "${CMAKE_CURRENT_LIST_DIR}/source/tbxmb_client.c"
)

target_include_directories(microtbx-modbus INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source"
)

# Create interface library for the template. Only used for MISRA check.
add_library(microtbx-modbus-template INTERFACE)

target_include_directories(microtbx-modbus-template INTERFACE 
    "${CMAKE_CURRENT_LIST_DIR}/source/template"
)

target_sources(microtbx-modbus-template INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/source/template/tbxmb_port.c"
)

