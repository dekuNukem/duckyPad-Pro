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
#include "bluetooth_task.h"
#include "nvs_flash.h"

#define PLUS_MINUS_BUTTON_COOLDOWN_MS 250
volatile uint8_t oled_brightness = OLED_CONTRAST_BRIGHT;
volatile uint32_t last_keypress;
volatile uint32_t last_execution_exit;

void block_until_anykey(uint8_t event_type)
{
  xQueueReset(switch_event_queue);
  while(1)
  {
    delay_ms(33);
    switch_event_t sw_event = { 0 };
    if(xQueueReceive(switch_event_queue, &sw_event, 0) == pdFALSE)
      continue;
    if(sw_event.type == event_type)
      return;
  }
}

void block_until_plus_minus_long_press(void)
{
  xQueueReset(switch_event_queue);
  while(1)
  {
    delay_ms(33);
    switch_event_t sw_event = { 0 };
    if(xQueueReceive(switch_event_queue, &sw_event, 0) == pdFALSE)
      continue;
    if((sw_event.id == SW_PLUS || sw_event.id == SW_MINUS) && sw_event.type == SW_EVENT_LONG_PRESS)
      return;
  }
}

uint8_t is_plus_minus_button(uint8_t swid)
{
  return swid == SW_MINUS || swid == SW_PLUS;
}

ds3_exe_result this_exe;

void der_init(ds3_exe_result* der)
{
  der->result = EXE_OK;
  der->next_pc = 0;
  der->data = 0;
  der->epilogue_actions = 0;
}

#define DSB_ALLOW_AUTOREPEAT 0
#define DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY 1
#define DSB_DONT_REPEAT_RETURN_IMMEDIATELY 2
uint8_t run_once(uint8_t swid, char* dsb_path, uint8_t* to_increment)
{
  der_init(&this_exe);
  run_dsb(&this_exe, swid, dsb_path);
  // printf("---\nexecution finished:\nresult: %d\ndata: %d\nepilogue: 0x%x\n---\n", this_exe.result, this_exe.data, this_exe.epilogue_actions);
  if(to_increment != NULL)
    *to_increment = *to_increment + 1;
  
  uint8_t what_to_do = DSB_ALLOW_AUTOREPEAT;

  if(this_exe.result == EXE_ABORTED)
  {
    neopixel_fill(128, 128, 128);
    oled_say("Aborted");
    delay_ms(100);
    media_key_release();
    delay_ms(100);
    keyboard_release_all();
    delay_ms(100);
    mouse_release_all();
    delay_ms(1000);
    goto_profile(current_profile_number);
    return DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY;
  }

  if(this_exe.epilogue_actions & EPILOGUE_SAVE_LOOP_STATE)
  {
    save_persistent_state(this_exe.epilogue_actions, swid);
  }
  if(this_exe.epilogue_actions & EPILOGUE_SAVE_COLOR_STATE)
  {
    save_persistent_state(this_exe.epilogue_actions, swid);
    what_to_do = DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY;
  }
  if(this_exe.epilogue_actions & EPILOGUE_NEED_OLED_RESTORE)
  {
    goto_profile_without_updating_rgb_LED(current_profile_number);
  }
  if(this_exe.epilogue_actions & EPILOGUE_DONT_AUTO_REPEAT)
  {
    what_to_do = DSB_DONT_REPEAT_RETURN_IMMEDIATELY;
  }

  if(this_exe.result >= EXE_ERROR_CODE_START)
  {
    neopixel_fill(128, 0, 0);
    draw_exe_error(this_exe.result);
    block_until_anykey(SW_EVENT_SHORT_PRESS);
    goto_profile(current_profile_number);
    return DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY;
  }
  else if(this_exe.result == EXE_ACTION_NEXT_PROFILE)
  {
    goto_next_profile();
    return DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY;
  }
  else if(this_exe.result == EXE_ACTION_PREV_PROFILE)
  {
    goto_prev_profile();
    return DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY;
  }
  else if(this_exe.result == EXE_ACTION_SLEEP)
  {
    start_sleeping();
    return DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY;
  }
  else if(this_exe.result == EXE_ACTION_GOTO_PROFILE)
  {
    goto_profile(this_exe.data);
    return DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY;
  }
  return what_to_do;
}

void onboard_offboard_switch_press(uint8_t swid, char* press_path, char* release_path)
{
  if(access(press_path, F_OK))
    return;
  play_keydown_animation(current_profile_number, swid);
  uint8_t* to_increment = &all_profile_info[current_profile_number].keypress_count[swid];
  //-------------
  uint8_t run_result = run_once(swid, press_path, to_increment);
  if(run_result == DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY)
    return;
  // don't repeat if on_release script exists
  if(access(release_path, F_OK) == 0)
    return;
  if(run_result == DSB_DONT_REPEAT_RETURN_IMMEDIATELY)
    goto handle_obsw_keydown_end;
  //--------------

  uint32_t hold_start = pdTICKS_TO_MS(xTaskGetTickCount());
  while(1)
  {
    if(poll_sw_state(swid, 1) == 0)
      goto handle_obsw_keydown_end;
    if(pdTICKS_TO_MS(xTaskGetTickCount())- hold_start > 500)
      break;
  }
  while(1)
  {
    if(poll_sw_state(swid, 1) == 0)
      break;
    uint8_t* to_increment = &all_profile_info[current_profile_number].keypress_count[swid];
    if(run_once(swid, press_path, to_increment) == DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY)
      return;
  }
  handle_obsw_keydown_end:
  // play keyup animation only if there is no on-release DSB file 
  if(access(release_path, F_OK))
    play_keyup_animation(current_profile_number, swid);
}

void settings_menu(void)
{
  ssd1306_set_rotation_only_for_128x128_do_not_use_for_anything_else(SSD1306_NO_ROTATION);
  draw_settings(&dp_settings);
  draw_settings_led();
  while(1)
  {
    switch_event_t sw_event = { 0 };
    delay_ms(20);
    if(xQueueReceive(switch_event_queue, &sw_event, 0) == pdFALSE)
      continue;
    if(sw_event.type != SW_EVENT_RELEASE)
      continue;

    // printf("settings_menu id: %d type: %d\n", sw_event.id, sw_event.type);
    if(sw_event.id == MSW_0)
    {
      dp_settings.brightness_index = (dp_settings.brightness_index + 1) % BRIGHTNESS_LEVEL_SIZE;
      draw_settings(&dp_settings);
      draw_settings_led();
    }
    else if(sw_event.id == MSW_1)
    {
      dp_settings.sleep_index = (dp_settings.sleep_index + 1) % SLEEP_OPTION_SIZE;
      draw_settings(&dp_settings);
    }
    else if(sw_event.id == MSW_2)
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
    }
    else if(sw_event.id == MSW_3)
    {
      dp_settings.bt_mode = (dp_settings.bt_mode + 1) % BT_MODE_SIZE;
      draw_settings(&dp_settings);
    }
    else if(sw_event.id == MSW_4)
    {
      neopixel_fill(0, 0, 128);
      nvs_flash_erase();
      draw_nvm_erase();
      block_until_anykey(SW_EVENT_SHORT_PRESS);
      esp_restart();
    }
    else if(sw_event.id == MSW_5)
    {
      generate_msc_flag_file();
      esp_restart();
    }
    else if(sw_event.id <= MAX_MSW)
    {
      break;
    }
  }
  save_settings(&dp_settings);
}

void onboard_offboard_switch_release(uint8_t swid, char* release_path)
{
  if(access(release_path, F_OK))
    return;
  run_once(swid, release_path, NULL);
  play_keyup_animation(current_profile_number, swid);
}

#define PATH_BUF_SIZE 128
char dsb_on_press_path_buf[PATH_BUF_SIZE];
char dsb_on_release_path_buf[PATH_BUF_SIZE];

void process_keyevent(uint8_t swid, uint8_t event_type)
{
  oled_brightness = OLED_CONTRAST_BRIGHT;
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
  if(is_plus_minus_button(swid) && event_type == SW_EVENT_LONG_PRESS)
  {
    settings_menu();
    goto_profile(current_profile_number);
    return;
  }
  if(is_plus_minus_button(swid))
    return; // just in case lol

  memset(dsb_on_press_path_buf, 0, PATH_BUF_SIZE);
  sprintf(dsb_on_press_path_buf, "/sdcard/%s/key%d.dsb", all_profile_info[current_profile_number].dir_path, swid+1);

  memset(dsb_on_release_path_buf, 0, PATH_BUF_SIZE);
  sprintf(dsb_on_release_path_buf, "/sdcard/%s/key%d-release.dsb", all_profile_info[current_profile_number].dir_path, swid+1);

  if(event_type == SW_EVENT_SHORT_PRESS)
    onboard_offboard_switch_press(swid, dsb_on_press_path_buf, dsb_on_release_path_buf);
  else if(event_type == SW_EVENT_RELEASE)
    onboard_offboard_switch_release(swid, dsb_on_release_path_buf);

  last_execution_exit = millis();
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
  update_last_keypress();
  delay_ms(20);
  is_sleeping = 0;
  oled_brightness = OLED_CONTRAST_BRIGHT;
}

void wakeup_from_sleep_and_load_profile(uint8_t profile_to_load)
{
  wakeup_from_sleep_no_load();
  goto_profile(profile_to_load);
}

void rotary_encoder_activity(uint8_t swid)
{
  memset(dsb_on_press_path_buf, 0, PATH_BUF_SIZE);
  sprintf(dsb_on_press_path_buf, "/sdcard/%s/key%d.dsb", all_profile_info[current_profile_number].dir_path, swid+1);
  if(access(dsb_on_press_path_buf, F_OK))
    return;
  all_profile_info[current_profile_number].keypress_count[swid]++;
  run_once(swid, dsb_on_press_path_buf, NULL);
}

void handle_rotary_encoder_event(rotary_encoder_event_t* this_re_event)
{
  update_last_keypress();
  if(is_sleeping)
  {
    wakeup_from_sleep_and_load_profile(current_profile_number);
    return;
  }
  oled_brightness = OLED_CONTRAST_BRIGHT;
  uint8_t swid = re_event_to_swid(this_re_event);
  if(swid == 0)
    return;
  rotary_encoder_activity(swid);
}



void handle_sw_event(switch_event_t* this_sw_event)
{
  update_last_keypress();
  // printf("swid: %d type: %d\n", this_sw_event->id, this_sw_event->type);
  if(is_sleeping && this_sw_event->type == SW_EVENT_SHORT_PRESS)
  {
    wakeup_from_sleep_and_load_profile(current_profile_number);
    return;
  }
  uint32_t ke_start = pdTICKS_TO_MS(xTaskGetTickCount());
  process_keyevent(this_sw_event->id, this_sw_event->type);
  uint32_t execution_duration = pdTICKS_TO_MS(xTaskGetTickCount()) - ke_start;
  // printf("took %ldms\n", execution_duration);
  if(execution_duration > 500)
    clear_sw_re_queue();
}

void keypress_task(void *dummy)
{
  update_last_keypress();
  while(1)
  {
    delay_ms(10);

    rotary_encoder_event_t re_event = { 0 };
    if (xQueueReceive(rotary_encoder_event_queue, &re_event, 0) == pdTRUE)
    {
      is_busy = 1;
      handle_rotary_encoder_event(&re_event);
      is_busy = 0;
    }

    switch_event_t sw_event = { 0 };
    if (xQueueReceive(switch_event_queue, &sw_event, 0) == pdTRUE)
    {
      if(is_plus_minus_button(sw_event.id) && millis() - last_execution_exit < PLUS_MINUS_BUTTON_COOLDOWN_MS)
        continue;
      is_busy = 1;
      handle_sw_event(&sw_event);
      is_busy = 0;
    }
    
    if(is_sleeping == 0)
    {
      update_bluetooth_icon(0, -1, bluetooth_status);
      update_kbled_icon(kb_led_status);
    }
    draw_bt_pin(bt_pin_code);
  }
}

