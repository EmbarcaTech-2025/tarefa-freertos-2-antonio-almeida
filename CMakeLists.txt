set(TARGET_NAME picow_freertos_ping_ac)
add_executable(${TARGET_NAME}
            src/picow_freertos_ping_ac.c
            src/ping.c
            src/i2c_setup.c
            src/oled_setup.c
            src/ssd1306.c
            )
    target_include_directories(${TARGET_NAME} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/include       # include ping.h in root
            ${CMAKE_CURRENT_LIST_DIR}/..            # for FreeRTOSConfig.h
            ${CMAKE_CURRENT_LIST_DIR}/../..         # for lwipopts.h
            )
    target_link_libraries(${TARGET_NAME}
            pico_cyw43_arch_lwip_sys_freertos
            pico_stdlib
            pico_lwip_iperf
            FreeRTOS-Kernel-Heap4                    # FreeRTOS kernel and dynamic heap
            hardware_i2c
            )
    
    pico_enable_stdio_usb(${TARGET_NAME} 1)
