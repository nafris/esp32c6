/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#define STACK_SIZE 10000
#define tskIDLE_PRIORITY 1



void vTask1Code(void * pVparameters)
{
    while(1){
        printf("Message from task 1...\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void createTask1(void){
    static uint8_t ucParameterToPass;
    TaskHandle_t handle_task1 = NULL;
    xTaskCreate(vTask1Code,"Task1",STACK_SIZE,&ucParameterToPass,tskIDLE_PRIORITY,&handle_task1);
    configASSERT(handle_task1);
}
void vTask2Code(void * pVparameters)
{
    while(1){
        printf("Message from task 2...\n");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void createTask2(void){
    static uint8_t ucParameterToPass;
    TaskHandle_t handle_task2 = NULL;
    xTaskCreate(vTask2Code,"Task2",STACK_SIZE,&ucParameterToPass,tskIDLE_PRIORITY,&handle_task2);
    configASSERT(handle_task2);
}



void app_main(void)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
    createTask1();
    createTask2();
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}