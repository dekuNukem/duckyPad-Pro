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
    switch_init();
    my_rotary_encoder_init();
    sd_init();
    oled_init();
    neopixel_init();

    while(1)
    {
        get_rc();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}