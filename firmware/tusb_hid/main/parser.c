#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "shared.h"
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

const char config_file_path[] = "/sdcard/dp_settings.txt";
dp_global_settings dp_settings;

char temp_buf[TEMP_BUFSIZE];

const char config_sleep_after_min[] = "sleep_after_min ";
const char config_brightness_index[] = "bi ";
const char config_keyboard_layout[] = "kbl ";

uint8_t current_profile_index;

int8_t load_settings(dp_global_settings* dps)
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
      for (int i = 0; i < FILENAME_BUFSIZE; ++i)
        if(dps->current_kb_layout[i] == '\r' || dps->current_kb_layout[i] == '\n')
          dps->current_kb_layout[i] = 0;
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

uint8_t available_profile_list[MAX_PROFILES];
const char* profile_dir_prefix = "profile";

uint8_t is_profile_dir(char* dirname)
{
  if(strncmp(dirname, profile_dir_prefix, strlen(profile_dir_prefix)))
    return 0;
  if(strlen(dirname) == strlen(profile_dir_prefix))
    return 0;
  char pnumber = dirname[strlen(profile_dir_prefix)];
  if(pnumber > '9' || pnumber < '0')
    return 0;
  if(strstr(dirname, "_") == 0)
    return 0;
  return 1;
}

void scan_profiles()
{
  struct dirent *dir;
  DIR *d = opendir(SD_MOUNT_POINT);
  memset(available_profile_list, 0, MAX_PROFILES);
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
      available_profile_list[this_profile_number] = 1;
    }
  }
  closedir(d);
}

void mytest(void)
{
  // printf("mytest: %d\n", get_last_used_profile());
  scan_profiles();
  for (int i = 0; i < MAX_PROFILES; i++)
  {
    if(available_profile_list[i])
      printf("%d\n", i);
  }
  
}