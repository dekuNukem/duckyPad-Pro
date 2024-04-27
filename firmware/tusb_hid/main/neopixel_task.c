#include "neopixel_task.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

const char *LED_TAG = "LED_TASK";

static led_strip_handle_t my_led_strip;

void neopixel_init(void)
{
    led_strip_config_t strip_config = {
    .strip_gpio_num = NEOPIXEL_PIN,
    .max_leds = NEOPIXEL_COUNT,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &my_led_strip));
    led_strip_clear(my_led_strip);

    for (int idx = 0; idx < NEOPIXEL_COUNT; ++idx)
        led_strip_set_pixel(my_led_strip, idx, 16, 32, 8);
    led_strip_refresh(my_led_strip);
}