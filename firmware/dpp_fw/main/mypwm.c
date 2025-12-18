#include "neopixel_task.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "profiles.h"
#include "freertos/semphr.h"
#include "ui_task.h"
#include "mypwm.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "sdkconfig.h"
#include "esp_pm.h"

const char *PWM_TAG = "PWM_TASK";

void example_ledc_init(void)
{
  // Prepare and then apply the LEDC PWM timer configuration
  ledc_timer_config_t ledc_timer = {
    .speed_mode       = LEDC_MODE,
    .duty_resolution  = LEDC_DUTY_RES,
    .timer_num        = LEDC_TIMER,
    .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
    .clk_cfg          = LEDC_CLK_SRC,
  };
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  // Prepare and then apply the LEDC PWM channel configuration
  ledc_channel_config_t ledc_channel = {
    .speed_mode     = LEDC_MODE,
    .channel        = LEDC_CHANNEL,
    .timer_sel      = LEDC_TIMER,
    .gpio_num       = LEDC_OUTPUT_IO,
    .duty           = 0, // Set duty to 0%
    .hpoint         = 0,
  };
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
  ESP_ERROR_CHECK(gpio_set_drive_capability(LEDC_OUTPUT_IO, GPIO_DRIVE_CAP_3));
}

void my_pwm_test(void)
{
  example_ledc_init();
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}