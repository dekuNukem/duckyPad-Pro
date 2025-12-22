#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sd_task.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#include "neopixel_task.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h> 

#include "profiles.h"
#include "shared.h"
#include "input_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "keyboard.h"
#include "ds_vm.h"
#include "bluetooth_task.h"

const char* profile_info_file_path = "/sdcard/profile_info.txt";
const char settings_file_path[] = "/sdcard/dpp_config.txt";
const char default_settings_file[] = "sleep_index 0\nbrightness_index 0\nlast_profile 1\nfw_ver 0.0.0\nserial_number DP24_000000\nkb_layout dpkm_English (US).txt\n";
dp_global_settings dp_settings;

char temp_buf[TEMP_BUFSIZE];
char filename_buf[FILENAME_BUFSIZE];

const char config_sleep_after_index[] = "sleep_index ";
const char config_brightness_index[] = "brightness_index ";
const char config_keyboard_layout[] = "kb_layout ";
const char config_last_used_profile[] = "last_profile ";
const char config_bt_mode[] = "bt_mode ";

const char cmd_BG_COLOR[] = "BG_COLOR ";
const char cmd_KD_COLOR[] = "KEYDOWN_COLOR ";
const char cmd_SWCOLOR[] = "SWCOLOR_";
const char cmd_DIM_UNUSED_KEYS[] = "DIM_UNUSED_KEYS 0";
const char cmd_IS_LANDSCAPE[] = "IS_LANDSCAPE 1";
const char cmd_IS_UPPER_HS[] = "UPPER_HS 1";
const char cmd_IS_LOWER_HS[] = "LOWER_HS 1";

uint8_t is_profile_load_complete;
uint8_t current_profile_number;
profile_info all_profile_info[MAX_PROFILES];

uint8_t load_settings(dp_global_settings* dps)
{
  if(dps == NULL)
    return 1;

  // remove incompatible files from old duckyPad
  CLEAR_TEMP_BUF();
  snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/dp_settings.txt");
  remove(temp_buf);
  CLEAR_TEMP_BUF();
  snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/dp_stats.txt");
  remove(temp_buf);

  memset(dps, 0, sizeof(*dps));
  FILE *sd_file;

  if(access(settings_file_path, F_OK))
  {
    sd_file = fopen(settings_file_path, "w");
    if(sd_file == NULL)
      return 2;
    fprintf(sd_file, "%s", default_settings_file);
    fclose(sd_file);
  }

  sd_file = fopen(settings_file_path, "r");
  if(sd_file == NULL)
    return 3;

  CLEAR_TEMP_BUF();
  memset(dps->current_kb_layout, 0, FILENAME_BUFSIZE);

  while(fgets(temp_buf, TEMP_BUFSIZE, sd_file))
  {
    if(strncmp(temp_buf, config_sleep_after_index, strlen(config_sleep_after_index)) == 0)
      dps->sleep_index = atoi(temp_buf + strlen(config_sleep_after_index));
    if(strncmp(temp_buf, config_brightness_index, strlen(config_brightness_index)) == 0)
      dps->brightness_index = atoi(temp_buf + strlen(config_brightness_index));
    if(strncmp(temp_buf, config_last_used_profile, strlen(config_last_used_profile)) == 0)
      dps->last_used_profile = atoi(temp_buf + strlen(config_last_used_profile));
    if(strncmp(temp_buf, config_bt_mode, strlen(config_bt_mode)) == 0)
      dps->bt_mode = atoi(temp_buf + strlen(config_bt_mode)) % BT_MODE_SIZE;
    if(dps->brightness_index >= BRIGHTNESS_LEVEL_SIZE)
      dps->brightness_index = BRIGHTNESS_LEVEL_SIZE - 1;
    if(strncmp(temp_buf, config_keyboard_layout, strlen(config_keyboard_layout)) == 0)
    {
      strcpy(dps->current_kb_layout, temp_buf + strlen(config_keyboard_layout));
      strip_newline(dps->current_kb_layout, FILENAME_BUFSIZE);
    }
  }
  fclose(sd_file);
  return 0;
}

uint8_t save_settings(dp_global_settings* dps)
{
  if(dps == NULL)
    return 1;
  FILE *sd_file = fopen(settings_file_path, "w");
  if(sd_file == NULL)
    return 2;
  fprintf(
    sd_file,
    "%s%d\n"
    "%s%d\n"
    "%s%d\n"
    "fw_ver %d.%d.%d\n"
    "serial_number DP24_%02X%02X%02X\n"
    "%s%s\n"
    "bt_mode %d\n",
    config_sleep_after_index, dps->sleep_index,
    config_brightness_index, dps->brightness_index,
    config_last_used_profile, current_profile_number,
    fw_version_major, fw_version_minor, fw_version_patch,
    esp_mac_addr[3], esp_mac_addr[4], esp_mac_addr[5],
    config_keyboard_layout, dps->current_kb_layout,
    dps->bt_mode
  );
  fclose(sd_file);
  return 0;
}

const char* profile_dir_prefix = "profile";

uint8_t is_profile_dir(char* dirname)
{
  if(strncmp(dirname, profile_dir_prefix, strlen(profile_dir_prefix)))
    return 0;
  if(strlen(dirname) == strlen(profile_dir_prefix))
    return 0;
  if(strlen(dirname) >= FILENAME_BUFSIZE)
    return 0;
  char pnumber = dirname[strlen(profile_dir_prefix)];
  if(pnumber > '9' || pnumber < '0')
    return 0;
  if(strstr(dirname, "_") == 0)
    return 0;
  return 1;
}

void load_profile_info(void)
{
  FILE *sd_file = fopen(profile_info_file_path, "r");
  uint8_t this_pf_number = 0;
  char* this_pf_name;
  if(sd_file == NULL)
    return;
  
  while(1)
  {
    CLEAR_TEMP_BUF();
    if(fgets(temp_buf, TEMP_BUFSIZE, sd_file) == NULL)
      break;
    this_pf_number = atoi(temp_buf);
    this_pf_name = goto_next_arg(temp_buf, temp_buf + PROFILE_NAME_MAX_LEN);
    if(this_pf_number == 0 || this_pf_number >= MAX_PROFILES)
      continue;
    if(this_pf_name == NULL)
      continue;
    strip_newline(this_pf_name, PROFILE_NAME_MAX_LEN);
    memset(filename_buf, 0, FILENAME_BUFSIZE);
    snprintf(filename_buf, FILENAME_BUFSIZE, "/sdcard/profile_%s/config.txt", this_pf_name);
    if(access(filename_buf, F_OK) != 0)
      continue;
    all_profile_info[this_pf_number].is_loaded = 1;
    memset(all_profile_info[this_pf_number].pf_name, 0, PROFILE_NAME_BUF_LEN);
    strncpy(all_profile_info[this_pf_number].pf_name, this_pf_name, PROFILE_NAME_MAX_LEN);
  }
  fclose(sd_file);
}

const char cmd_sw_name_firstline[] = "z";
const char cmd_sw_name_secondline[] = "x";

void parse_profile_config_line(char* this_line, profile_info* this_profile)
{
  if(this_line == NULL || strlen(this_line) <= 2)
    return;
  
  char* msg_end = this_line + strlen(this_line);

  if(strncmp(cmd_sw_name_firstline, this_line, strlen(cmd_sw_name_firstline)) == 0)
  {
    uint8_t this_key_index = atoi(this_line + strlen(cmd_sw_name_firstline));
    if(this_key_index == 0)
      return;
    this_key_index--;
    if(this_key_index >= TOTAL_OBSW_COUNT)
      return;
    memset(this_profile->sw_name_firstline[this_key_index], 0, KEYNAME_SIZE);
    char* kn_start = goto_next_arg(this_line, msg_end);
    if(kn_start == NULL)
      return;
    strncpy(this_profile->sw_name_firstline[this_key_index], kn_start, KEYNAME_SIZE-1);
    this_profile->sw_name_firstline[this_key_index][5] = 0;
  }
  else if(strncmp(cmd_sw_name_secondline, this_line, strlen(cmd_sw_name_secondline)) == 0)
  {
    uint8_t this_key_index = atoi(this_line + strlen(cmd_sw_name_secondline));
    if(this_key_index == 0)
      return;
    this_key_index--;
    if(this_key_index >= TOTAL_OBSW_COUNT)
      return;
    memset(this_profile->sw_name_secondline[this_key_index], 0, KEYNAME_SIZE);
    char* kn_start = goto_next_arg(this_line, msg_end);
    if(kn_start == NULL)
      return;
    strncpy(this_profile->sw_name_secondline[this_key_index], kn_start, KEYNAME_SIZE-1);
    this_profile->sw_name_secondline[this_key_index][5] = 0;
  }
  else if(strncmp(cmd_BG_COLOR, this_line, strlen(cmd_BG_COLOR)) == 0) // order is important! BG, SW, and KD
  {
    char* curr = goto_next_arg(this_line, msg_end);
    uint8_t rrr = atoi(curr);
    curr = goto_next_arg(curr, msg_end);
    uint8_t ggg = atoi(curr);
    curr = goto_next_arg(curr, msg_end);
    uint8_t bbb = atoi(curr);
    for (size_t i = 0; i < MECH_OBSW_COUNT; i++)
    {
      this_profile->sw_color_default[i][0] = rrr;
      this_profile->sw_color_default[i][1] = ggg;
      this_profile->sw_color_default[i][2] = bbb;
      this_profile->sw_color_keydown[i][0] = 255 - rrr;
      this_profile->sw_color_keydown[i][1] = 255 - ggg;
      this_profile->sw_color_keydown[i][2] = 255 - bbb;
    }
  }
  else if(strncmp(cmd_SWCOLOR, this_line, strlen(cmd_SWCOLOR)) == 0)
  {
    char* curr = this_line + strlen(cmd_SWCOLOR);
    uint8_t sw_index = atoi(curr) - 1;
    if(sw_index >= MECH_OBSW_COUNT)
      return;
    curr = goto_next_arg(curr, msg_end);
    uint8_t rrr = atoi(curr);
    curr = goto_next_arg(curr, msg_end);
    uint8_t ggg = atoi(curr);
    curr = goto_next_arg(curr, msg_end);
    uint8_t bbb = atoi(curr);
    this_profile->sw_color_default[sw_index][0] = rrr;
    this_profile->sw_color_default[sw_index][1] = ggg;
    this_profile->sw_color_default[sw_index][2] = bbb;
    this_profile->sw_color_keydown[sw_index][0] = 255 - rrr;
    this_profile->sw_color_keydown[sw_index][1] = 255 - ggg;
    this_profile->sw_color_keydown[sw_index][2] = 255 - bbb;
  }
  else if(strncmp(cmd_KD_COLOR, this_line, strlen(cmd_KD_COLOR)) == 0)
  {
    char* curr = goto_next_arg(this_line, msg_end);
    uint8_t rrr = atoi(curr);
    curr = goto_next_arg(curr, msg_end);
    uint8_t ggg = atoi(curr);
    curr = goto_next_arg(curr, msg_end);
    uint8_t bbb = atoi(curr);
    for (size_t i = 0; i < MECH_OBSW_COUNT; i++)
    {
      this_profile->sw_color_keydown[i][0] = rrr;
      this_profile->sw_color_keydown[i][1] = ggg;
      this_profile->sw_color_keydown[i][2] = bbb;
    }
  }
  else if(strncmp(cmd_DIM_UNUSED_KEYS, this_line, strlen(cmd_DIM_UNUSED_KEYS)) == 0)
  {
    this_profile->dim_unused_keys = 0;
  }
  else if(strncmp(cmd_IS_LANDSCAPE, this_line, strlen(cmd_IS_LANDSCAPE)) == 0)
  {
    this_profile->is_landscape = 1;
  }
  else if(strncmp(cmd_IS_UPPER_HS, this_line, strlen(cmd_IS_UPPER_HS)) == 0)
  {
    this_profile->is_upper_halfstep = 1;
  }
  else if(strncmp(cmd_IS_LOWER_HS, this_line, strlen(cmd_IS_LOWER_HS)) == 0)
  {
    this_profile->is_lower_halfstep = 1;
  }
}

void load_profile_config(profile_info* this_profile)
{
  if(this_profile == NULL || this_profile->is_loaded == 0)
    return;
  memset(filename_buf, 0, FILENAME_BUFSIZE);
  snprintf(filename_buf, FILENAME_BUFSIZE, "%s/profile_%s/config.txt", SD_MOUNT_POINT, this_profile->pf_name);

  FILE *sd_file = fopen(filename_buf, "r");
  if(sd_file == NULL)
    return;
  this_profile->dim_unused_keys = 1;
  CLEAR_TEMP_BUF();
  while(fgets(temp_buf, TEMP_BUFSIZE, sd_file))
  {
    strip_newline(temp_buf, TEMP_BUFSIZE);
    parse_profile_config_line(temp_buf, this_profile);
    CLEAR_TEMP_BUF();
  }

  if(this_profile->dim_unused_keys)
  {
    for (size_t i = 0; i < MECH_OBSW_COUNT; i++)
    {
      if(strlen(this_profile->sw_name_firstline[i]) > 0)
        continue;
      this_profile->sw_color_default[i][0] = 0;
      this_profile->sw_color_default[i][1] = 0;
      this_profile->sw_color_default[i][2] = 0;
      this_profile->sw_color_keydown[i][0] = 0;
      this_profile->sw_color_keydown[i][1] = 0;
      this_profile->sw_color_keydown[i][2] = 0;
    }
  }
  fclose(sd_file);
}

uint8_t is_valid_profile_number(uint8_t profile_number)
{
  if(profile_number >= MAX_PROFILES || all_profile_info[profile_number].is_loaded == 0)
    return 0;
  return 1;
}

uint8_t scan_profiles()
{
  memset(all_profile_info, 0, sizeof(all_profile_info));
  load_profile_info();

  uint8_t has_valid_profile = 0;
  for (size_t i = 0; i < MAX_PROFILES; i++)
    has_valid_profile += all_profile_info[i].is_loaded;
  if(has_valid_profile == 0)
    return PSCAN_ERROR_NO_PROFILE;

  for(size_t i = 0; i < MAX_PROFILES; i++)
    load_profile_config(&all_profile_info[i]);
  
  return PSCAN_OK;
}

uint8_t goto_profile_without_updating_rgb_LED(uint8_t profile_number)
{
  if(is_valid_profile_number(profile_number) == 0)
    return 1;
  draw_profile(&all_profile_info[profile_number]);
  current_profile_number = profile_number;
  save_settings(&dp_settings);
  return 0;
}

void goto_profile(uint8_t profile_number)
{
  if(goto_profile_without_updating_rgb_LED(profile_number))
    return;
  if(load_persistent_state())
    redraw_bg(profile_number);
  else
    neopixel_draw_current_buffer();
  
  if(all_profile_info[profile_number].is_upper_halfstep)
    set_re_halfstep(1, 1);
  else
    set_re_halfstep(1, 0);
  
  if(all_profile_info[profile_number].is_lower_halfstep)
    set_re_halfstep(0, 1);
  else
    set_re_halfstep(0, 0);
}

void goto_next_profile(void)
{
  uint8_t new_profile_number = current_profile_number;
  while(1)
  {
    new_profile_number++;
    if(new_profile_number >= MAX_PROFILES)
      new_profile_number = 0;
    if(all_profile_info[new_profile_number].is_loaded)
      break;
  }
  goto_profile(new_profile_number);
}

void goto_prev_profile(void)
{
  uint8_t new_profile_number = current_profile_number;
  while(1)
  {
    if(new_profile_number == 0)
      new_profile_number = MAX_PROFILES - 1;
    else
      new_profile_number--;
    if(all_profile_info[new_profile_number].is_loaded)
      break;
  }
  goto_profile(new_profile_number);
}

void profile_init(void)
{
  if(is_valid_profile_number(dp_settings.last_used_profile))
    goto_profile(dp_settings.last_used_profile);
  else
    goto_next_profile();
}

const char* dk_circumflex = "dk_circumflex";
const char* dk_diaeresis = "dk_diaeresis";
const char* dk_grave_accent = "dk_grave_accent";
const char* dk_acute_accent = "dk_acute_accent";
const char* dk_tilde = "dk_tilde";
const char* dk_cedilla = "dk_cedilla";

uint8_t load_keymap_by_name(char* km_name)
{
  char* next;
  if(km_name == NULL)
    return 1;
  CLEAR_TEMP_BUF();
  snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/keymaps/%s", km_name);
  FILE *sd_file = fopen(temp_buf, "r");
  if(sd_file == NULL)
    return ERROR_KEYMAP_NOT_FOUND;
  
  while(fgets(temp_buf, TEMP_BUFSIZE, sd_file))
  {
    if(strncmp(temp_buf, dk_circumflex, strlen(dk_circumflex)) == 0)
      duckcode_circumflex = strtoul(temp_buf + strlen(dk_circumflex), NULL, 0);
    else if(strncmp(temp_buf, dk_diaeresis, strlen(dk_diaeresis)) == 0)
      duckcode_diaeresis = strtoul(temp_buf + strlen(dk_diaeresis), NULL, 0);
    else if(strncmp(temp_buf, dk_grave_accent, strlen(dk_grave_accent)) == 0)
      duckcode_grave_accent = strtoul(temp_buf + strlen(dk_grave_accent), NULL, 0);
    else if(strncmp(temp_buf, dk_acute_accent, strlen(dk_acute_accent)) == 0)
      duckcode_acute_accent = strtoul(temp_buf + strlen(dk_acute_accent), NULL, 0);
    else if(strncmp(temp_buf, dk_tilde, strlen(dk_tilde)) == 0)
      duckcode_tilde = strtoul(temp_buf + strlen(dk_tilde), NULL, 0);
    else if(strncmp(temp_buf, dk_cedilla, strlen(dk_cedilla)) == 0)
      duckcode_cedilla = strtoul(temp_buf + strlen(dk_cedilla), NULL, 0);

    uint8_t ascii_index = strtoul(temp_buf, &next, 0);
    uint16_t keycode = strtoul(next, NULL, 0);
    ascii_map[ascii_index] = keycode;
  }
  fclose(sd_file);
  ascii_map[0] = 0;
  return 0;
}

uint8_t get_next_keymap(const char* current_keymap_filename, char* next_keymap_filename)
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

uint8_t get_first_keymap(char* keymap_filename)
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

void idle_loop(void)
{
  while(1)
  {
    delay_ms(1000);
  }
}

uint8_t should_mount_usb_msc(void)
{
  uint8_t found = 0;
  struct dirent *dir;
  DIR *d = opendir(SD_MOUNT_POINT);
  if(d == NULL)
    return found;

  while ((dir = readdir(d)) != NULL)
  {
    if(dir->d_type != DT_REG)
      continue;
    if(strncmp(dir->d_name, "MOUNTMSC.TXT", 12) == 0)
    {
      found = 1;
      break;
    }
  }
  closedir(d);
  return found;
}

void delete_msc_flag_file(void)
{
  remove("/sdcard/MOUNTMSC.TXT");
}

void generate_msc_flag_file(void)
{
  FILE* mff = fopen("/sdcard/MOUNTMSC.TXT", "w");
  fclose(mff);
}

#define SPS_COLOR_START_ADDR (SPS_HEADER_SIZE + MAX_TOTAL_SW_COUNT)
#define SPS_PROTOCOL_VERSION 1
#define SPS_HEADER_SIZE 4
#define SPS_BIN_SIZE 256
uint8_t sps_bin_buf[SPS_BIN_SIZE];

/*
  Persistent state file:
  Byte 0: D
  Byte 1: P
  Byte 2: SPS Version
  Byte 3: LED state start address
  Byte 4+: Keypress count for LOOP command
  LED state: starts at addr indicated on byte 3 

  LED state: 4 bytes per chunk
  Byte 0: has_user_assigned_color
  Byte 1: Red
  Byte 2: Green
  Byte 3: Blue
*/

void save_persistent_state(void)
{
  memset(sps_bin_buf, 0, SPS_BIN_SIZE);
  sps_bin_buf[0] = 'D';
  sps_bin_buf[1] = 'P';
  sps_bin_buf[2] = SPS_PROTOCOL_VERSION;
  sps_bin_buf[3] = SPS_COLOR_START_ADDR;
  memcpy(sps_bin_buf+SPS_HEADER_SIZE, all_profile_info[current_profile_number].keypress_count, MAX_TOTAL_SW_COUNT);
  for (uint16_t i = 0; i < NEOPIXEL_COUNT; i++)
  {
    uint16_t has_user_assigned_color_addr = i*4 + SPS_COLOR_START_ADDR;
    uint16_t r_addr = has_user_assigned_color_addr + 1;
    uint16_t g_addr = r_addr + 1;
    uint16_t b_addr = g_addr + 1;
    uint8_t has_user_assigned_color = all_profile_info[current_profile_number].has_user_assigned_keycolor[i];
    if(has_user_assigned_color == 0)
      continue;
    if(b_addr >= SPS_BIN_SIZE)
      break;
    sps_bin_buf[has_user_assigned_color_addr] = has_user_assigned_color;
    sps_bin_buf[r_addr] = all_profile_info[current_profile_number].sw_color_user_assigned[i][RED];
    sps_bin_buf[g_addr] = all_profile_info[current_profile_number].sw_color_user_assigned[i][GREEN];
    sps_bin_buf[b_addr] = all_profile_info[current_profile_number].sw_color_user_assigned[i][BLUE];
  }
  CLEAR_TEMP_BUF();
  snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/profile_%s/state_dpp.sps", all_profile_info[current_profile_number].pf_name);
  FILE *file = fopen(temp_buf, "wb");
  if(file == NULL)
    return;
  fwrite(sps_bin_buf, 1, SPS_BIN_SIZE, file);
  fclose(file);
}

uint8_t load_persistent_state(void)
{
  CLEAR_TEMP_BUF();
  snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/profile_%s/state_dpp.sps", all_profile_info[current_profile_number].pf_name);

  FILE *file = fopen(temp_buf, "rb");
  if (file == NULL)
    return 1;
  
  memset(sps_bin_buf, 0, SPS_BIN_SIZE);
  size_t read_bytes = fread(sps_bin_buf, 1, SPS_BIN_SIZE, file);
  fclose(file);

  if (read_bytes < SPS_HEADER_SIZE)
    return 2;
  if (sps_bin_buf[0] != 'D' || sps_bin_buf[1] != 'P')
    return 3;
  if(sps_bin_buf[2] != SPS_PROTOCOL_VERSION)
    return 4;

  uint8_t color_start_addr = sps_bin_buf[3];
  memcpy(all_profile_info[current_profile_number].keypress_count, sps_bin_buf + SPS_HEADER_SIZE, MAX_TOTAL_SW_COUNT);
  memset(all_profile_info[current_profile_number].has_user_assigned_keycolor, 0, MECH_OBSW_COUNT);

  for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
  {
    uint16_t has_user_assigned_color_addr = i * 4 + color_start_addr;
    if (has_user_assigned_color_addr + 3 >= SPS_BIN_SIZE)
      break;
    uint8_t has_user_color = sps_bin_buf[has_user_assigned_color_addr];
    if(has_user_color == 0)
      continue;
    all_profile_info[current_profile_number].has_user_assigned_keycolor[i] = has_user_color;
    all_profile_info[current_profile_number].sw_color_user_assigned[i][RED]   = sps_bin_buf[has_user_assigned_color_addr + 1];
    all_profile_info[current_profile_number].sw_color_user_assigned[i][GREEN] = sps_bin_buf[has_user_assigned_color_addr + 2];
    all_profile_info[current_profile_number].sw_color_user_assigned[i][BLUE]  = sps_bin_buf[has_user_assigned_color_addr + 3];
  }
  return 0;
}

void save_gv(void)
{
  memset(sps_bin_buf, 0, SPS_BIN_SIZE);
  memcpy(sps_bin_buf, pgv_buf, PGV_COUNT*sizeof(uint32_t));
  CLEAR_TEMP_BUF();
  snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/gv.sps");
  FILE *file = fopen(temp_buf, "wb");
  fwrite(sps_bin_buf, 1, SPS_BIN_SIZE, file);
  fclose(file);
}

void load_gv(void)
{
  CLEAR_TEMP_BUF();
  snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/gv.sps");
  FILE *file = fopen(temp_buf, "rb");
  if(file == NULL)
    return;
  memset(sps_bin_buf, 0, SPS_BIN_SIZE);
  fread(sps_bin_buf, 1, SPS_BIN_SIZE, file);
  fclose(file);
  memcpy(pgv_buf, sps_bin_buf, PGV_COUNT * sizeof(uint32_t));
}

const char* profile_str = "profile";
uint8_t ensure_new_profile_format(void)
{
  char* this_profile_name;
  uint8_t this_profile_number;
  struct stat st;

  if(access(profile_info_file_path, F_OK) == 0) // profile_info.txt already exists
    return 1;
  
  DIR* dir = opendir("/sdcard/");
  if(dir == NULL)
    return 2;

  oled_say("Converting...");
  memset(all_profile_info, 0, sizeof(all_profile_info));
  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL)
  {
    memset(filename_buf, 0, FILENAME_BUFSIZE);
    strncpy(filename_buf, entry->d_name, FILENAME_BUFSIZE - 1);
    CLEAR_TEMP_BUF();
    snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/%s", filename_buf);
    if(stat(temp_buf, &st) == -1)
      continue;
    if(!S_ISDIR(st.st_mode))
      continue;
    if(strncmp(filename_buf, profile_str, strlen(profile_str)) != 0)
      continue;

    printf("Converting: %s\n", filename_buf);
    this_profile_number = atoi(filename_buf + strlen(profile_str));
    if(this_profile_number >= MAX_PROFILES)
      continue;
    this_profile_name = strchr(filename_buf, '_');
    if(this_profile_name == NULL)
      continue;
    this_profile_name++;
    all_profile_info[this_profile_number].is_loaded = 1;
    memset(all_profile_info[this_profile_number].pf_name, 0, PROFILE_NAME_BUF_LEN);
    strncpy(all_profile_info[this_profile_number].pf_name, this_profile_name, PROFILE_NAME_MAX_LEN);
  }
  closedir(dir);

  FILE *sd_file = fopen(profile_info_file_path, "w");
  if(sd_file == NULL)
    return 3;
  for (size_t i = 0; i < MAX_PROFILES; i++)
  {
    if(all_profile_info[i].is_loaded == 0)
      continue;
    fprintf(sd_file, "%d %s\n", i, all_profile_info[i].pf_name);
  }
  fclose(sd_file);

  for (size_t i = 0; i < MAX_PROFILES; i++)
  {
    if(all_profile_info[i].is_loaded == 0)
      continue;
    CLEAR_TEMP_BUF();
    memset(filename_buf, 0, FILENAME_BUFSIZE);
    snprintf(temp_buf, TEMP_BUFSIZE, "/sdcard/profile%d_%s", i, all_profile_info[i].pf_name);
    snprintf(filename_buf, FILENAME_BUFSIZE, "/sdcard/profile_%s", all_profile_info[i].pf_name);
    printf("%s --> %s\n", temp_buf, filename_buf);
    rename(temp_buf, filename_buf);
  }
  oled_say("Done!");
  return 0;
}