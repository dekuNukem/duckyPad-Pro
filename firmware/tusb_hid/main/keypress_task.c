#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"

#include "rotary_encoder.h"
#include "input_task.h"
#include "sd_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "shared.h"
#include "profiles.h"
#include "ssd1306.h"
#include "esp_mac.h"
#include "keypress_task.h"
#include "unistd.h"

uint32_t last_keypress;

void block_until_anykey(void)
{
  xQueueReset(switch_event_queue);
  while(1)
  {
    vTaskDelay(pdMS_TO_TICKS(33));
    switch_event_t sw_event = { 0 };
    if(xQueueReceive(switch_event_queue, &sw_event, 0) == pdFALSE)
      continue;
    if(sw_event.type == SW_EVENT_SHORT_PRESS)
      return;
  }
}

void handle_keydown(uint8_t swid)
{
  memset(temp_buf, 0, TEMP_BUFSIZE);
  sprintf(temp_buf, "/sdcard/%s/key%d.dsb", all_profile_info[current_profile_number].dir_path, swid+1);
  if(access(temp_buf, F_OK))
  {
    draw_red();
    draw_nodsb(swid);
    block_until_anykey();
    goto_profile(current_profile_number);
    return;
  }
  play_keydown_animation(current_profile_number, swid);
  vTaskDelay(pdMS_TO_TICKS(33)); // placeholder
  play_keyup_animation(current_profile_number, swid);
}

// void handle_keyup(uint8_t swid)
// {
//   play_keyup_animation(current_profile_number, swid);
// }

void settings_menu(void)
{
  draw_settings(&dp_settings);
  while(1)
  {
    switch_event_t sw_event = { 0 };
    if (xQueueReceive(switch_event_queue, &sw_event, 0) == pdTRUE)
    {
      printf("settings_menu id: %d type: %d\n", sw_event.id, sw_event.type);
      if(sw_event.id == MSW_0 && sw_event.type == SW_EVENT_SHORT_PRESS)
      {
        dp_settings.brightness_index = (dp_settings.brightness_index + 1) % BRIGHTNESS_LEVEL_SIZE;
        draw_settings(&dp_settings);
        neopixel_draw_current_buffer();
      }

      if((sw_event.id == SW_MINUS || sw_event.id == SW_PLUS) && sw_event.type == SW_EVENT_SHORT_PRESS)
      {
        return;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void handle_keyevent(uint8_t swid, uint8_t event_type)
{
  if(swid == SW_PLUS && event_type == SW_EVENT_RELEASE)
  {
    goto_next_profile();
    return;
  }
  if(swid == SW_MINUS && event_type == SW_EVENT_RELEASE)
  {
    goto_prev_profile();
    return;
  }
  if((swid == SW_PLUS || swid == SW_MINUS) && event_type == SW_EVENT_LONG_PRESS)
  {
    settings_menu();
    redraw_bg(current_profile_number);
    return;
  }

  if(swid >= MSW_0 && swid <= MAX_MSW)
  {
    if(event_type == SW_EVENT_SHORT_PRESS)
    {
      handle_keydown(swid);
    }
    else if(event_type == SW_EVENT_RELEASE)
    {
      // handle_keyup(swid);
    }
  }
}

void keypress_task(void *dummy)
{
  last_keypress = pdTICKS_TO_MS(xTaskGetTickCount());
  while(1)
  {
    rotary_encoder_event_t re_event = { 0 };
    if (xQueueReceive(rotary_encoder_event_queue, &re_event, 0) == pdTRUE)
    {
      printf("Event: id: %d pos: %ld, dir: %d\n", re_event.state.id, re_event.state.position, re_event.state.direction);
      last_keypress = pdTICKS_TO_MS(xTaskGetTickCount());
      if(re_event.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE)
        goto_next_profile();
      else if(re_event.state.direction == ROTARY_ENCODER_DIRECTION_COUNTER_CLOCKWISE)
        goto_prev_profile();
      ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
    }

    switch_event_t sw_event = { 0 };
    if (xQueueReceive(switch_event_queue, &sw_event, 0) == pdTRUE)
    {
      printf("id: %d type: %d\n", sw_event.id, sw_event.type);
      handle_keyevent(sw_event.id, sw_event.type);
      last_keypress = pdTICKS_TO_MS(xTaskGetTickCount());
      ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
    }
    vTaskDelay(pdMS_TO_TICKS(25));
  }
}

