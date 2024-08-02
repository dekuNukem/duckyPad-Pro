#include "neopixel_task.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "profiles.h"
#include "freertos/semphr.h"

const char *LED_TAG = "LED_TASK";

led_strip_handle_t my_led_strip;

uint32_t frame_counter;
uint8_t pixel_map[NEOPIXEL_COUNT] = {3,2,1,0,4,5,6,7,11,10,9,8,12,13,14,15,19,18,17,16};
uint8_t red_buf[NEOPIXEL_COUNT];
uint8_t green_buf[NEOPIXEL_COUNT];
uint8_t blue_buf[NEOPIXEL_COUNT];
led_animation neo_anime[NEOPIXEL_COUNT];
uint8_t color_red[THREE] = {64 , 0, 0};
uint8_t brightness_index_to_percent_lookup[BRIGHTNESS_LEVEL_SIZE] = {0, 20, 50, 70, 100};
SemaphoreHandle_t neopixel_mutex;

void set_pixel_3color(uint8_t which, uint8_t r, uint8_t g, uint8_t b)
{
  red_buf[pixel_map[which]] = r;
  green_buf[pixel_map[which]] = g;
  blue_buf[pixel_map[which]] = b;
}

void set_pixel_3color_update_buffer(uint8_t which, uint8_t r, uint8_t g, uint8_t b)
{
  neo_anime[which].animation_type = ANIMATION_NONE;
  set_pixel_3color(which, r, g, b);
  neo_anime[which].current_color[0] = r;
  neo_anime[which].current_color[1] = g;
  neo_anime[which].current_color[2] = b;
  neo_anime[which].target_color[0] = r;
  neo_anime[which].target_color[1] = g;
  neo_anime[which].target_color[2] = b;
}

void set_pixel_color(uint8_t which, uint8_t dest_color[THREE])
{
  set_pixel_3color(which, dest_color[0], dest_color[1], dest_color[2]);
}

uint8_t scale_by_brightness(uint8_t value, uint8_t percent)
{
  return value * (double)percent / 100;
}

void neopixel_show(uint8_t* red, uint8_t* green, uint8_t* blue, uint8_t brightness_percentage)
{
  for (int idx = 0; idx < NEOPIXEL_COUNT; ++idx)
    led_strip_set_pixel(my_led_strip, idx, scale_by_brightness(red[idx], brightness_percentage), scale_by_brightness(green[idx], brightness_percentage), scale_by_brightness(blue[idx], brightness_percentage));
  led_strip_refresh(my_led_strip);
}

void neopixel_draw_current_buffer(void)
{
  if(xSemaphoreTake(neopixel_mutex, pdMS_TO_TICKS(NEOPIXEL_MUTEX_TIMEOUT_MS)) == pdFALSE)
    return;
  neopixel_show(red_buf, green_buf, blue_buf, brightness_index_to_percent_lookup[dp_settings.brightness_index]);
  xSemaphoreGive(neopixel_mutex);
}

void draw_red(void)
{
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
  {
    neo_anime[i].animation_type = ANIMATION_NONE;
    set_pixel_3color(i, 255, 0, 0);
  }
  neopixel_draw_current_buffer();
}

void neopixel_off(void)
{
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
  {
    neo_anime[i].animation_type = ANIMATION_NONE;
    set_pixel_3color(i, 0, 0, 0);
  }
  neopixel_draw_current_buffer();
}

void redraw_bg(uint8_t profile_number)
{
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
  {
    neo_anime[i].animation_type = ANIMATION_NONE;
    set_pixel_color(i, all_profile_info[profile_number].sw_color[i]);
  }
  neopixel_draw_current_buffer();
}

void reset_key_color(uint8_t which)
{
  if(which >= NEOPIXEL_COUNT)
    return;
  neo_anime[which].animation_type = ANIMATION_NONE;
  set_pixel_color(which, all_profile_info[current_profile_number].sw_color[which]);
  neopixel_draw_current_buffer();
}

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

  neopixel_mutex = xSemaphoreCreateMutex();
}

void led_start_animation(led_animation* anime_struct, uint8_t dest_color[THREE], uint8_t anime_type, uint8_t durations_frames)
{
  for (int i = 0; i < THREE; ++i)
    anime_struct->step[i] = (dest_color[i] - anime_struct->current_color[i]) / (double)durations_frames;
  memcpy(anime_struct->target_color, dest_color, THREE);
  anime_struct->animation_start = frame_counter;
  anime_struct->animation_type = anime_type;
  anime_struct->animation_duration = durations_frames;
}

void play_keydown_animation(uint8_t profile_number, uint8_t sw_number)
{
  if(sw_number > MAX_MSW)
    return;
  set_pixel_color(sw_number, all_profile_info[profile_number].sw_activation_color[sw_number]);
  neo_anime[sw_number].current_color[0] = all_profile_info[profile_number].sw_activation_color[sw_number][0];
  neo_anime[sw_number].current_color[1] = all_profile_info[profile_number].sw_activation_color[sw_number][1];
  neo_anime[sw_number].current_color[2] = all_profile_info[profile_number].sw_activation_color[sw_number][2];
  neo_anime[sw_number].target_color[0] = all_profile_info[profile_number].sw_activation_color[sw_number][0];
  neo_anime[sw_number].target_color[1] = all_profile_info[profile_number].sw_activation_color[sw_number][1];
  neo_anime[sw_number].target_color[2] = all_profile_info[profile_number].sw_activation_color[sw_number][2];
  neopixel_draw_current_buffer();
}

void play_keyup_animation(uint8_t profile_number, uint8_t sw_number)
{
  if(sw_number > MAX_MSW)
    return;
  led_start_animation(&neo_anime[sw_number], all_profile_info[profile_number].sw_color[sw_number], ANIMATION_CROSS_FADE, 50);
}

void led_animation_handler(void)
{
  frame_counter++;
  uint8_t needs_update = 0;
  for (int idx = 0; idx < NEOPIXEL_COUNT; idx++)
  {
    int32_t current_frame = frame_counter - neo_anime[idx].animation_start;
    if(current_frame <= 0)
      continue;
    if(neo_anime[idx].animation_type != ANIMATION_CROSS_FADE)
      continue;

    if(current_frame <= neo_anime[idx].animation_duration)
    {
      for (int i = 0; i < THREE; ++i)
      {
        neo_anime[idx].current_color[i] += neo_anime[idx].step[i];
        if(neo_anime[idx].current_color[i] > 255)
          neo_anime[idx].current_color[i] = 255;
        if(neo_anime[idx].current_color[i] < 0)
          neo_anime[idx].current_color[i] = 0;
      }
    }
    else
    {
      for (int i = 0; i < THREE; ++i)
        neo_anime[idx].current_color[i] = neo_anime[idx].target_color[i];
      neo_anime[idx].animation_type = ANIMATION_NONE;
    }
    needs_update = 1;
    set_pixel_3color(idx, (uint8_t)neo_anime[idx].current_color[0], (uint8_t)neo_anime[idx].current_color[1], (uint8_t)neo_anime[idx].current_color[2]);
  }
  if(needs_update && xSemaphoreTake(neopixel_mutex, pdMS_TO_TICKS(NEOPIXEL_MUTEX_TIMEOUT_MS)))
  {
    neopixel_show(red_buf, green_buf, blue_buf, brightness_index_to_percent_lookup[dp_settings.brightness_index]);
    xSemaphoreGive(neopixel_mutex);
  }
}

void led_animation_init()
{
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
  {
    for (int cc = 0; cc < THREE; cc++)
    {
      neo_anime[i].current_color[cc] = 0;
      neo_anime[i].step[cc] = 0;
      neo_anime[i].target_color[cc] = 0;
    }
    neo_anime[i].animation_start = 0;
    neo_anime[i].animation_duration = 0;
    neo_anime[i].animation_type = ANIMATION_NONE;
  }
}

void draw_settings_led(void)
{
  neopixel_off();

  set_pixel_3color(0, 0, 0, 255);
  set_pixel_3color(1, 0, 0, 255);
  set_pixel_3color(2, 0, 0, 255);
  
  set_pixel_3color(16, 0, 255, 0);
  set_pixel_3color(17, 0, 255, 0);
  set_pixel_3color(18, 0, 255, 0);
  set_pixel_3color(19, 0, 255, 0);
  neopixel_draw_current_buffer();
}

void get_current_color(uint8_t which, uint8_t* red, uint8_t* green, uint8_t* blue)
{
  *red = red_buf[pixel_map[which]];
  *green = green_buf[pixel_map[which]];
  *blue = blue_buf[pixel_map[which]];
}
