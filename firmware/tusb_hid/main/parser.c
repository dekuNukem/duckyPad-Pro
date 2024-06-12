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
#include "ui_task.h"

const char config_file_path[] = "/sdcard/dp_settings.txt";
dp_global_settings dp_settings;

char temp_buf[TEMP_BUFSIZE];
char filename_buf[FILENAME_BUFSIZE];

const char config_sleep_after_min[] = "sleep_after_min ";
const char config_brightness_index[] = "bi ";
const char config_keyboard_layout[] = "kbl ";

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
  printf("name: %s\n", pinfo->name);
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
      all_profile_info[count].name = all_profile_info[count].dir_path + strlen(profile_dir_prefix) + how_many_digits(this_profile_number) + 1;
      print_profile_info(&all_profile_info[count]);

      count++;
    }
  }
  closedir(d);
}

uint8_t scan_profiles()
{
  valid_profile_count = get_valid_profile_count();
  if(valid_profile_count == 0)    
    return PSCAN_ERROR_NO_PROFILE;

  printf("%s: found %d valid profiles\n", __func__, valid_profile_count);
  
  free(all_profile_info);
  all_profile_info = (profile_info *)malloc(valid_profile_count * sizeof(profile_info));
  if(all_profile_info == NULL)
    return PSCAN_ERROR_NO_MEMORY;
  
  memset(all_profile_info, 0, valid_profile_count * sizeof(profile_info));
  fill_profile_info();
  return PSCAN_OK;
}

void mytest(void)
{
  uint8_t last_used_profile = get_last_used_profile();
}