#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"

#include "input_task.h"
#include "sd_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "shared.h"
#include "parser.h"
#include "ssd1306.h"

uint8_t fw_version_major = 0;
uint8_t fw_version_minor = 0;
uint8_t fw_version_patch = 1;

static const char *TAG = "MAIN";

/*
    xTaskCreate higher number = higher priority
    configMAX_PRIORITIES
*/

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(500)); // brief delay in case of SD card removal reboot
    gpio_install_isr_service(0); // BEFORE GPIO INIT

    my_rotary_encoder_init();
    switch_init();
    oled_init();
    if(sd_init())
    {
        print_nosd();
        while(1){vTaskDelay(pdMS_TO_TICKS(250));}
    }
    
    neopixel_init();
    xTaskCreate(kb_scan_task, "kb_scan_task", SW_SCAN_TASK_STACK_SIZE, NULL, 2, NULL);

    load_settings(&dp_settings);

    while(1)
    {
        get_rc();
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}