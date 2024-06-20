#include "neopixel_task.h"
#include "led_strip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "profiles.h"

const char *LED_TAG = "LED_TASK";

static led_strip_handle_t my_led_strip;

uint32_t frame_counter;
uint8_t pixel_map[NEOPIXEL_COUNT] = {3,2,1,0,4,5,6,7,11,10,9,8,12,13,14,15,19,18,17,16};
uint8_t red_buf[NEOPIXEL_COUNT];
uint8_t green_buf[NEOPIXEL_COUNT];
uint8_t blue_buf[NEOPIXEL_COUNT];
led_animation neo_anime[NEOPIXEL_COUNT];
uint8_t color_red[THREE] = {64 , 0, 0};
uint8_t color_black[THREE] = {0, 0, 0};
uint8_t brightness_values[BRIGHTNESS_LEVELS] = {0, 20, 50, 70, 100};

void set_pixel_3color(uint8_t which, uint8_t r, uint8_t g, uint8_t b)
{
  red_buf[pixel_map[which]] = r;
  green_buf[pixel_map[which]] = g;
  blue_buf[pixel_map[which]] = b;
}

void set_pixel_color(uint8_t which, uint8_t dest_color[THREE])
{
  set_pixel_3color(which, dest_color[0], dest_color[1], dest_color[2]);
}

void neopixel_show(uint8_t* red, uint8_t* green, uint8_t* blue, uint8_t brightness_percentage)
{
  for (int idx = 0; idx < NEOPIXEL_COUNT; ++idx)
    led_strip_set_pixel(my_led_strip, idx, red[idx], green[idx], blue[idx]);
  led_strip_refresh(my_led_strip);
}

void redraw_bg(uint8_t profile_number)
{
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
  {
    neo_anime[i].animation_type = ANIMATION_NONE;
    set_pixel_color(i, all_profile_info[profile_number].sw_color[i]);
  }
  neopixel_show(red_buf, green_buf, blue_buf, brightness_values[dp_settings.brightness_index]);
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

    for (int idx = 0; idx < NEOPIXEL_COUNT; ++idx)
        led_strip_set_pixel(my_led_strip, idx, 8, 2, 8);
    led_strip_refresh(my_led_strip);
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
  if(sw_number >= MECH_OBSW_COUNT)
    return;
  set_pixel_color(sw_number, all_profile_info[profile_number].sw_activation_color[sw_number]);
  neo_anime[sw_number].current_color[0] = all_profile_info[profile_number].sw_activation_color[sw_number][0];
  neo_anime[sw_number].current_color[1] = all_profile_info[profile_number].sw_activation_color[sw_number][1];
  neo_anime[sw_number].current_color[2] = all_profile_info[profile_number].sw_activation_color[sw_number][2];
  neo_anime[sw_number].target_color[0] = all_profile_info[profile_number].sw_activation_color[sw_number][0];
  neo_anime[sw_number].target_color[1] = all_profile_info[profile_number].sw_activation_color[sw_number][1];
  neo_anime[sw_number].target_color[2] = all_profile_info[profile_number].sw_activation_color[sw_number][2];
  neopixel_show(red_buf, green_buf, blue_buf, brightness_values[dp_settings.brightness_index]);
}

void play_keyup_animation(uint8_t profile_number, uint8_t sw_number)
{
  if(sw_number >= MECH_OBSW_COUNT)
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
  if(needs_update)
    neopixel_show(red_buf, green_buf, blue_buf, brightness_values[dp_settings.brightness_index]);
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