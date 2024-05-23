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

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_ERROR_CHECK(gpio_install_isr_service(0)); // BEFORE GPIO INIT
    my_rotary_encoder_init();
    switch_init();
    sd_init();
    oled_init();
    neopixel_init();

    while(1)
    {
        input_test();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}