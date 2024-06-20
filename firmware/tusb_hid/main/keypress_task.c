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

void handle_keyup(uint8_t swid)
{
  if(swid == SW_PLUS)
    goto_next_profile();
  else if(swid == SW_MINUS)
    goto_prev_profile();
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
      printf("id: %d lvl: %d\n", sw_event.id, sw_event.level);
      if(sw_event.level) // key down
        handle_keydown(sw_event.id);
      else
        handle_keyup(sw_event.id);
    }
    vTaskDelay(pdMS_TO_TICKS(25));
  }
}

