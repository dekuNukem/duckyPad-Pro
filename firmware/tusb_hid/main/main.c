#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"

#include "input_task.h"
#include "sd_task.h"
#include "ui_task.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    gpio_init();
    sd_init();
    oled_init();
    while(1)
    {
        // printf("write test\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
