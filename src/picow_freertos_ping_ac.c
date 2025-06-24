/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Adapted for Google BDL AC ping
 */

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/ip4_addr.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ping.h"

#include "i2c_setup.h"
#include "oled_setup.h"
#include "ssd1306.h"

#ifndef PING_ADDR
#define PING_ADDR "8.8.8.8" // Default address for pinging Google Public DNS
#endif
#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif

#define TEST_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)

void main_task(__unused void *params) // Main task to initialize Wi-Fi and start pinging
{
    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return;
    }
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect.\n");
        exit(1);
    }
    else
    {
        printf("Connected.\n");
    }

    ip_addr_t ping_addr;
    ipaddr_aton(PING_ADDR, &ping_addr);
    ping_init(&ping_addr);

    while (true)
    {
        // not much to do as LED is in another task, and we're using RAW (callback) lwIP API
        vTaskDelay(100);
    }

    cyw43_arch_deinit();
}

void oled_ping_task(void *pvParameters) // Task to handle OLED updates
{
    (void)pvParameters;

    while (1)
    {
        if (oled_needs_update)
        {
            update_oled_text(linha1_buf, linha2_buf); // writes on OLED
            oled_needs_update = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // sleep 100ms
    }
}

void vLaunch(void)
{
    TaskHandle_t task;
    /* Create the main task that will run on core 0. */
    xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);

    
    // Note: The OLED task is created with a higher priority than the main task
    xTaskCreate(oled_ping_task, "OLED Task", 512, NULL, tskIDLE_PRIORITY + 1, NULL);

#if NO_SYS && configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1
    // we must bind the main task to one core (well at least while the init is called)
    // (note we only do this in NO_SYS mode, because cyw43_arch_freertos
    // takes care of it otherwise)
    vTaskCoreAffinitySet(task, 1);
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main(void)
{
    stdio_init_all();
    setup_i2c();  //  initialize do I2C
    setup_oled(); // initialize OLED of BDL

    /* Configure the hardware ready to run the demo. */
    const char *rtos_name;
#if (configNUMBER_OF_CORES > 1)
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if (configNUMBER_OF_CORES == 2) // look at this... If we have two cores, we can run FreeRTOS on both
    printf("Starting %s on both cores:\n", rtos_name);
    vLaunch();
#elif (RUN_FREERTOS_ON_CORE == 1)
    printf("Starting %s on core 1:\n", rtos_name);
    multicore_launch_core1(vLaunch);
    while (true)
        ;
#else
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();
#endif
    return 0;
}
