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

#include "ssd1306.h"

uint8_t fw_version_major = 0;
uint8_t fw_version_minor = 0;
uint8_t fw_version_patch = 1;

static const char *TAG = "MAIN";

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(500)); // brief delay in case of SD card removal reboot
    gpio_install_isr_service(0); // BEFORE GPIO INIT

    // my_rotary_encoder_init();
    switch_init();
    oled_init();
    if (sd_init())
        print_nosd();
    neopixel_init();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}