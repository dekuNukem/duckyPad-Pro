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

#include "parser.h"
#include "shared.h"
#include "input_task.h"
#include "ui_task.h"

const char config_file_path[] = "/sdcard/dp_settings.txt";
dp_global_settings dp_settings;

char temp_buf[TEMP_BUFSIZE];
char filename_buf[FILENAME_BUFSIZE];

const char config_sleep_after_min[] = "sleep_after_min ";
const char config_brightness_index[] = "bi ";
const char config_keyboard_layout[] = "kbl ";

const char cmd_BG_COLOR[] = "BG_COLOR ";
const char cmd_KD_COLOR[] = "KEYDOWN_COLOR ";
const char cmd_SWCOLOR[] = "SWCOLOR_";
const char cmd_DIM_UNUSED_KEYS[] = "DIM_UNUSED_KEYS ";

uint8_t current_profile_index;
profile_info *all_profile_info;
uint8_t valid_profile_count;

uint8_t load_settings(dp_global_settings* dps)
{
  if(dps == NULL)
    return 1;

  FILE *sd_file = fopen(config_file_path, "r");
  if(sd_file == NULL)
    return 2;

  memset(temp_buf, 0, TEMP_BUFSIZE);
  memset(dps->current_kb_layout, 0, FILENAME_BUFSIZE);

  while(fgets(temp_buf, TEMP_BUFSIZE, sd_file))
  {
    // printf("tb: %s\n", temp_buf);
    if(strncmp(temp_buf, config_sleep_after_min, strlen(config_sleep_after_min)) == 0)
      dps->sleep_after_ms = atoi(temp_buf + strlen(config_sleep_after_min)) * 60000;
    if(strncmp(temp_buf, config_brightness_index, strlen(config_brightness_index)) == 0)
      dps->brightness_index = atoi(temp_buf + strlen(config_brightness_index));
    if(dps->brightness_index >= BRIGHTNESS_LEVELS)
      dps->brightness_index = BRIGHTNESS_LEVELS - 1;
    if(strncmp(temp_buf, config_keyboard_layout, strlen(config_keyboard_layout)) == 0)
    {
      strcpy(dps->current_kb_layout, temp_buf + strlen(config_keyboard_layout));
      strip_newline(dps->current_kb_layout, FILENAME_BUFSIZE);
    }
  }
  fclose(sd_file);
  return 0;
}

const char stat_last_used_profile[] = "lp ";
uint8_t get_last_used_profile(void)
{
  uint8_t ret = 0;

  FILE *sd_file = fopen("/sdcard/dp_stats.txt", "r");
  if(sd_file == NULL)
    return 1;

  memset(temp_buf, 0, TEMP_BUFSIZE);
  while(fgets(temp_buf, TEMP_BUFSIZE, sd_file))
  {
    if(strncmp(temp_buf, stat_last_used_profile, strlen(stat_last_used_profile)) == 0)
      ret = atoi(temp_buf+strlen(stat_last_used_profile));
  }

  if(ret >= MAX_PROFILES)
    ret = 0;

  fclose(sd_file);
  return ret;
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

uint8_t unique_profiles[MAX_PROFILES];
uint8_t get_valid_profile_count()
{
  uint8_t count = 0;
  struct dirent *dir;
  DIR *d = opendir(SD_MOUNT_POINT);
  memset(unique_profiles, 0, MAX_PROFILES);
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
      unique_profiles[this_profile_number] = 1;
    }
  }
  closedir(d);

  for (uint8_t i = 0; i < MAX_PROFILES; i++)
    if(unique_profiles[i])
      count++;
  
  return count;
}

void print_profile_info(profile_info *pinfo)
{
  if(pinfo == NULL)
    return;
  printf("--------\n");
  printf("index: %d\n", pinfo->index);
  printf("is_loaded: %d\n", pinfo->is_loaded);
  printf("dir_path: %s\n", pinfo->dir_path);
  printf("pf_name: %s\n", pinfo->pf_name);
  printf("dim_unused_keys: %d\n", pinfo->dim_unused_keys);
  for (size_t i = 0; i < TOTAL_OBSW_COUNT; i++)
  {
    if(strlen(pinfo->sw_name[i]) == 0)
      continue;
    printf("key %d: %s\n", i, pinfo->sw_name[i]);
    printf("sw_color %d %d %d\n", pinfo->sw_color[i][0], pinfo->sw_color[i][1], pinfo->sw_color[i][2]);
    printf("sw_activation_color %d %d %d\n", pinfo->sw_activation_color[i][0], pinfo->sw_activation_color[i][1], pinfo->sw_activation_color[i][2]);
    printf(".\n");
  }
  printf("--------\n");
}

void fill_profile_info(void)
{
  uint8_t count = 0;
  struct dirent *dir;
  DIR *d = opendir(SD_MOUNT_POINT);
  memset(unique_profiles, 0, MAX_PROFILES);
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

      // if two profile folders have same number, dont load twice
      if(all_profile_info[count].is_loaded || count >= valid_profile_count)
        continue;
      all_profile_info[count].index = this_profile_number;
      all_profile_info[count].is_loaded = 1;
      memset(all_profile_info[count].dir_path, 0, FILENAME_BUFSIZE);
      strcpy(all_profile_info[count].dir_path, dir->d_name);
      all_profile_info[count].pf_name = all_profile_info[count].dir_path + strlen(profile_dir_prefix) + how_many_digits(this_profile_number) + 1;
      count++;
    }
  }
  closedir(d);
}

void parse_profile_config_line(char* this_line, profile_info* this_profile)
{
  char* msg_end = this_line + strlen(this_line);

  if(this_line == NULL || strlen(this_line) <= 2)
    return;

  if(this_line[0] == 'z')
  {
    uint8_t this_key_index = atoi(this_line+1);
    if(this_key_index == 0)
      return;
    this_key_index--;
    if(this_key_index >= TOTAL_OBSW_COUNT)
      return;
    memset(this_profile->sw_name[this_key_index], 0, KEYNAME_SIZE);
    char* kn_start = goto_next_arg(this_line, msg_end);
    if(kn_start == NULL)
      return;
    strcpy(this_profile->sw_name[this_key_index], kn_start);
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
      this_profile->sw_color[i][0] = rrr;
      this_profile->sw_color[i][1] = ggg;
      this_profile->sw_color[i][2] = bbb;
      this_profile->sw_activation_color[i][0] = 255 - rrr;
      this_profile->sw_activation_color[i][1] = 255 - ggg;
      this_profile->sw_activation_color[i][2] = 255 - bbb;
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
    this_profile->sw_color[sw_index][0] = rrr;
    this_profile->sw_color[sw_index][1] = ggg;
    this_profile->sw_color[sw_index][2] = bbb;
    this_profile->sw_activation_color[sw_index][0] = 255 - rrr;
    this_profile->sw_activation_color[sw_index][1] = 255 - ggg;
    this_profile->sw_activation_color[sw_index][2] = 255 - bbb;
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
      this_profile->sw_activation_color[i][0] = rrr;
      this_profile->sw_activation_color[i][1] = ggg;
      this_profile->sw_activation_color[i][2] = bbb;
    }
  }
  else if(strncmp(cmd_DIM_UNUSED_KEYS, this_line, strlen(cmd_DIM_UNUSED_KEYS)) == 0)
  {
    char* curr = goto_next_arg(this_line, msg_end);
    this_profile->dim_unused_keys = atoi(curr);
  }
}

void load_profile_config(profile_info* this_profile)
{
  if(this_profile == NULL)
    return;
  memset(filename_buf, 0, FILENAME_BUFSIZE);
  sprintf(filename_buf, "%s/%s/config.txt", SD_MOUNT_POINT, this_profile->dir_path);

  FILE *sd_file = fopen(filename_buf, "r");
  if(sd_file == NULL)
    return;
  memset(temp_buf, 0, TEMP_BUFSIZE);
  while(fgets(temp_buf, TEMP_BUFSIZE, sd_file))
  {
    strip_newline(temp_buf, TEMP_BUFSIZE);
    parse_profile_config_line(temp_buf, this_profile);
  }
  // print_profile_info(this_profile);
  fclose(sd_file);
}

uint8_t scan_profiles()
{
  valid_profile_count = get_valid_profile_count();
  if(valid_profile_count == 0)    
    return PSCAN_ERROR_NO_PROFILE;

  uint32_t bytes_to_allocate = valid_profile_count * sizeof(profile_info);
  printf("%s: Found %d profiles, allocating %ld bytes\n", __func__, valid_profile_count, bytes_to_allocate);
  
  free(all_profile_info);
  all_profile_info = (profile_info *)malloc(bytes_to_allocate);
  if(all_profile_info == NULL)
    return PSCAN_ERROR_NO_MEMORY;
  
  memset(all_profile_info, 0, bytes_to_allocate);
  fill_profile_info();

  for (size_t i = 0; i < valid_profile_count; i++)
    load_profile_config(&all_profile_info[i]);

  return PSCAN_OK;
}


void mytest(void)
{
  draw_profile(&all_profile_info[0]);
}