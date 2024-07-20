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
#include "keyboard.h"
#include "ds_vm.h"
#include <dirent.h>
#include "hid_task.h"

volatile uint32_t last_keypress;

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

void der_init(ds3_exe_result* der)
{
  der->result = EXE_EMPTY_FILE;
  der->next_pc = 0;
  der->data = 0;
  der->data2 = 0;
  der->epilogue_actions = 0;
}

ds3_exe_result this_exe;

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
  //-------------
  der_init(&this_exe);
  run_dsb(&this_exe, swid, temp_buf);
  // app_send_hid_demo();
  //--------------
  play_keyup_animation(current_profile_number, swid);
}

// void handle_keyup(uint8_t swid)
// {
//   play_keyup_animation(current_profile_number, swid);
// }

void settings_menu(void)
{
  draw_settings(&dp_settings);
  draw_settings_led();
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
        draw_settings_led();
      }
      if(sw_event.id == MSW_2 && sw_event.type == SW_EVENT_SHORT_PRESS)
      {
        memset(temp_buf, 0, TEMP_BUFSIZE);
        if(get_next_keymap(dp_settings.current_kb_layout, temp_buf))
        {
          memset(dp_settings.current_kb_layout, 0, FILENAME_BUFSIZE);
          get_first_keymap(dp_settings.current_kb_layout);
        }
        else
        {
          strcpy(dp_settings.current_kb_layout, temp_buf);
        }
        draw_settings(&dp_settings);
        save_settings(&dp_settings);
      }
      if((sw_event.id >= MSW_16 && sw_event.id <= MSW_19) && sw_event.type == SW_EVENT_SHORT_PRESS)
      {
        return;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void process_keyevent(uint8_t swid, uint8_t event_type)
{
  ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
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
    goto_profile(current_profile_number);
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

volatile uint8_t is_sleeping;
volatile uint8_t is_busy;

void start_sleeping(void)
{
  if(is_sleeping)
    return;
  neopixel_off();
  ssd1306_Fill(Black);
  ssd1306_UpdateScreen();
  is_sleeping = 1;
}

void update_last_keypress(void)
{
  last_keypress = pdTICKS_TO_MS(xTaskGetTickCount());
}

void wakeup_from_sleep_no_load(void)
{
  is_sleeping = 0;
  ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
  update_last_keypress();
}

void wakeup_from_sleep_and_load_profile(uint8_t profile_to_load)
{
  wakeup_from_sleep_no_load();
  goto_profile(profile_to_load);
}

void handle_rotary_encoder_event(rotary_encoder_event_t* this_re_event)
{
  update_last_keypress();
  printf("reid: %d pos: %ld, dir: %d\n", this_re_event->state.id, this_re_event->state.position, this_re_event->state.direction);
  if(is_sleeping)
  {
    wakeup_from_sleep_and_load_profile(current_profile_number);
    return;
  }
  ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
  if(this_re_event->state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE)
    goto_next_profile();
  else if(this_re_event->state.direction == ROTARY_ENCODER_DIRECTION_COUNTER_CLOCKWISE)
    goto_prev_profile();
}

void handle_sw_event(switch_event_t* this_sw_event)
{
  update_last_keypress();
  printf("swid: %d type: %d\n", this_sw_event->id, this_sw_event->type);
  if(is_sleeping)
  {
    wakeup_from_sleep_and_load_profile(current_profile_number);
    return;
  }
  ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
  is_busy = 1;
  process_keyevent(this_sw_event->id, this_sw_event->type);
  is_busy = 0;
}

void keypress_task(void *dummy)
{
  update_last_keypress();
  while(1)
  {
    rotary_encoder_event_t re_event = { 0 };
    if (xQueueReceive(rotary_encoder_event_queue, &re_event, 0) == pdTRUE)
      handle_rotary_encoder_event(&re_event);

    switch_event_t sw_event = { 0 };
    if (xQueueReceive(switch_event_queue, &sw_event, 0) == pdTRUE)
      handle_sw_event(&sw_event);
    vTaskDelay(pdMS_TO_TICKS(25));
  }
}

