# Create target for MISRA checking MicroTBX-Modbus if cppcheck can be located.
find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    # Collect MicroTBX include directories.
    get_target_property(microtbx_incs microtbx INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX Cortex-M port include directories.
    get_target_property(microtbx_cortexm_incs microtbx-cortexm INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX template include directories.
    get_target_property(microtbx_template_incs microtbx-template INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX-Modbus include directories.
    get_target_property(microtbx_modbus_incs microtbx-modbus INTERFACE_INCLUDE_DIRECTORIES)
    # Collect MicroTBX-Modbus template include directories.
    get_target_property(microtbx_modbus_template_incs microtbx-modbus-template INTERFACE_INCLUDE_DIRECTORIES)

    # Build list with search paths for include files and transform by adding a "-I".
    set(search_path_incs)
    list(APPEND search_path_incs ${microtbx_incs})
    list(APPEND search_path_incs ${microtbx_cortexm_incs})
    list(APPEND search_path_incs ${microtbx_template_incs})
    list(APPEND search_path_incs ${microtbx_modbus_incs})
    list(APPEND search_path_incs ${microtbx_modbus_template_incs})
    list(TRANSFORM search_path_incs PREPEND "-I")

    # Collect MicroTBX-Modbus sources.
    get_target_property(microtbx_modbus_srcs microtbx-modbus INTERFACE_SOURCES)
    # Collect MicroTBX-Modbus template sources.
    get_target_property(microtbx_modbus_template_srcs microtbx-modbus-template INTERFACE_SOURCES)
    # Collect MicroTBX-Modbus OSAL superloop sources.
    get_target_property(microtbx_modbus_superloop_srcs microtbx-modbus-osal-superloop INTERFACE_SOURCES)
    # Collect MicroTBX-Modbus OSAL freertos sources.
    get_target_property(microtbx_modbus_freertos_srcs microtbx-modbus-osal-freertos INTERFACE_SOURCES)

    # Build list with MicroTBX-Modbus sources to check.
    set(check_srcs)
    list(APPEND check_srcs ${microtbx_modbus_srcs})
    list(APPEND check_srcs ${microtbx_modbus_template_srcs})
    list(APPEND check_srcs ${microtbx_modbus_superloop_srcs})
    list(APPEND check_srcs ${microtbx_modbus_freertos_srcs})

    # Set variable pointing to the addon for configuring the MISRA checks.
    set(misra_addon "${CMAKE_CURRENT_LIST_DIR}/misra.json")

    # Build list with cppcheck commands, one for each given source file
    set(cppcheck_commands)
    foreach(sourcefile ${check_srcs})
        # only include C-source files
        if( sourcefile MATCHES \\.c$ )
            # create command line for running cppcheck on one source file and add it
            # to the list of commands
            list(APPEND cppcheck_commands
                COMMAND ${CMAKE_CXX_CPPCHECK}
                --platform=arm32-wchar_t2.xml
                --addon=${misra_addon}
                --relative-paths
                --error-exitcode=1
                --language=c
                --std=c11
                --inline-suppr 
                --suppress=missingIncludeSystem
                --suppress=unmatchedSuppression
                --enable=warning,style,information,missingInclude
                -DDEBUG
                ${search_path_incs}
                ${sourcefile})
        endif()
    endforeach(sourcefile)

    # add a custom target consisting of all the commands generated above
    add_custom_target(microtbx_modbus_MISRA ${cppcheck_commands} VERBATIM)
endif()
