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
  // memset(temp_buf, 0, TEMP_BUFSIZE);
  // sprintf(temp_buf, "hello world");
  // kb_print(temp_buf, 20, 0);
  der_init(&this_exe);
  run_dsb(&this_exe, swid, temp_buf);
  //--------------
  play_keyup_animation(current_profile_number, swid);
}

// void handle_keyup(uint8_t swid)
// {
//   play_keyup_animation(current_profile_number, swid);
// }

/*
void load_profile_info(void)
{
  struct dirent *dir;
  DIR *d = opendir(SD_MOUNT_POINT);
  if(d) 
  {
    while ((dir = readdir(d)) != NULL)
    {
      if(dir->d_type != DT_DIR)
        continue;
      if(is_profile_dir(dir->d_name) == 0)
        continue;
      uint8_t this_profile_number = atoi(dir->d_name + strlen(profile_dir_prefix));
      if(this_profile_number >= MAX_PROFILES)
        continue;

      all_profile_info[this_profile_number].pf_number = this_profile_number;
      all_profile_info[this_profile_number].is_loaded = 1;
      memset(all_profile_info[this_profile_number].dir_path, 0, FILENAME_BUFSIZE);
      strcpy(all_profile_info[this_profile_number].dir_path, dir->d_name);
      all_profile_info[this_profile_number].pf_name = all_profile_info[this_profile_number].dir_path + strlen(profile_dir_prefix) + how_many_digits(this_profile_number) + 1;
    }
  }
  closedir(d);
}
*/

#define ERROR_NO_KEYMAP_FOLDER 1
#define ERROR_KEYMAP_NOT_FOUND 2

uint8_t get_next_keymap(char* current_keymap_filename, char* next_keymap_filename, uint8_t next_keymap_buf_size)
{
  struct dirent *dir;
  DIR *d = opendir("/sdcard/keymaps");
  if(d == NULL)
    return ERROR_NO_KEYMAP_FOLDER;
  uint8_t found = 0;
  while ((dir = readdir(d)) != NULL)
  {
    if(dir->d_type != DT_REG)
      continue;
    if(!(strncmp(dir->d_name, "dpkm_", 5) == 0 && strstr(dir->d_name, ".txt") != NULL))
      continue;
    if(found)
    {
      strcpy(next_keymap_filename, dir->d_name);
      closedir(d);
      return 0;
    }
    if(strcmp(dir->d_name, current_keymap_filename) == 0)
      found = 1;
  }
  closedir(d);
  return ERROR_KEYMAP_NOT_FOUND;
}

uint8_t get_first_keymap(char* keymap_filename, uint8_t keymap_buf_size)
{
  struct dirent *dir;
  DIR *d = opendir("/sdcard/keymaps");
  if(d == NULL)
    return ERROR_NO_KEYMAP_FOLDER;
  while ((dir = readdir(d)) != NULL)
  {
    if(dir->d_type != DT_REG)
      continue;
    if(!(strncmp(dir->d_name, "dpkm_", 5) == 0 && strstr(dir->d_name, ".txt") != NULL))
      continue;
    strcpy(keymap_filename, dir->d_name);
    closedir(d);
    return 0;
  }
  closedir(d);
  return ERROR_KEYMAP_NOT_FOUND;
}

void settings_menu(void)
{
  draw_settings(&dp_settings);
  sprintf(dp_settings.current_kb_layout, "dpkm_French.txt");
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
      if(sw_event.id == MSW_19 && sw_event.type == SW_EVENT_SHORT_PRESS)
      {
        // memset(temp_buf, 0, TEMP_BUFSIZE);
        // get_next_keymap(dp_settings.current_kb_layout, temp_buf, TEMP_BUFSIZE);
        // strcpy(dp_settings.current_kb_layout, temp_buf);
        // printf("current keymap: %s\n", temp_buf);

        memset(temp_buf, 0, TEMP_BUFSIZE);
        get_first_keymap(temp_buf, TEMP_BUFSIZE);
        printf("first keymap: %s\n", temp_buf);

        // draw_settings(&dp_settings);
        // neopixel_draw_current_buffer();
      }
      if((sw_event.id == SW_MINUS || sw_event.id == SW_PLUS) && sw_event.type == SW_EVENT_SHORT_PRESS)
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

void wakeup_from_sleep(void)
{
  is_sleeping = 0;
  ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
  goto_profile(current_profile_number);
}

void handle_rotary_encoder_event(rotary_encoder_event_t* this_re_event)
{
  last_keypress = pdTICKS_TO_MS(xTaskGetTickCount());
  printf("reid: %d pos: %ld, dir: %d\n", this_re_event->state.id, this_re_event->state.position, this_re_event->state.direction);
  if(is_sleeping)
  {
    wakeup_from_sleep();
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
  last_keypress = pdTICKS_TO_MS(xTaskGetTickCount());
  printf("swid: %d type: %d\n", this_sw_event->id, this_sw_event->type);
  if(is_sleeping)
  {
    wakeup_from_sleep();
    return;
  }
  ssd1306_SetContrast(OLED_CONTRAST_BRIGHT);
  is_busy = 1;
  process_keyevent(this_sw_event->id, this_sw_event->type);
  is_busy = 0;
}

void keypress_task(void *dummy)
{
  last_keypress = pdTICKS_TO_MS(xTaskGetTickCount());
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

