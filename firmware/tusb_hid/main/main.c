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

#include "led_strip.h"
#define NEOPIXEL_PIN 46
static led_strip_handle_t led_strip;
#define NEOPIXEL_COUNT 3

void app_main(void)
{
    gpio_init();
    sd_init();
    oled_init();

    led_strip_config_t strip_config = {
    .strip_gpio_num = NEOPIXEL_PIN,
    .max_leds = NEOPIXEL_COUNT,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);

    for (int idx = 0; idx < NEOPIXEL_COUNT; ++idx)
        led_strip_set_pixel(led_strip, idx, 64, 32, 16);
    led_strip_refresh(led_strip);

    while(1)
    {
        // printf("write test\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
