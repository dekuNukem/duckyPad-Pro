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

void handle_keydown(uint8_t swid)
{
  play_keydown_animation(current_profile_number, swid);
}

void handle_keyup(uint8_t swid, uint8_t event_type)
{
  play_keyup_animation(current_profile_number, swid);
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
    draw_settings();
    return;
  }
  
  if(swid == SW_PLUS || swid == SW_MINUS)
    return;

  if(event_type == SW_EVENT_SHORT_PRESS)
    play_keydown_animation(current_profile_number, swid);
  else
    play_keyup_animation(current_profile_number, swid);
}

void keypress_task(void *dummy)
{
  while(1)
  {
    rotary_encoder_event_t re_event = { 0 };
    if (xQueueReceive(rotary_encoder_event_queue, &re_event, 0) == pdTRUE)
    {
      printf("Event: id: %d pos: %ld, dir: %d\n", re_event.state.id, re_event.state.position, re_event.state.direction);
      if(re_event.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE)
        goto_next_profile();
      else if(re_event.state.direction == ROTARY_ENCODER_DIRECTION_COUNTER_CLOCKWISE)
        goto_prev_profile();
    }

    switch_event_t sw_event = { 0 };
    if (xQueueReceive(switch_event_queue, &sw_event, 0) == pdTRUE)
    {
      printf("id: %d type: %d\n", sw_event.id, sw_event.type);
      handle_keyevent(sw_event.id, sw_event.type);
    }
    vTaskDelay(pdMS_TO_TICKS(25));
  }
}

