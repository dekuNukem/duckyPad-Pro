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

#include "parser.h"

const char config_file_path[] = "/sdcard/dp_settings.txt";
FIL* sd_file;
dp_global_settings dp_settings;

char temp_buf[TEMP_BUFSIZE];

const char config_sleep_after_min[] = "sleep_after_min ";
const char config_brightness_index[] = "bi ";
const char config_keyboard_layout[] = "kbl ";

void load_settings(dp_global_settings* dps)
{
  if(dps == NULL)
    return;
  if(f_open(&sd_file, config_file_path, FA_READ) != 0)
    goto ggs_end;
  memset(temp_buf, 0, TEMP_BUFSIZE);
  memset(dps->current_kb_layout, 0, FILENAME_BUFSIZE);
  while(f_gets(temp_buf, TEMP_BUFSIZE, &sd_file))
  {
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
  ggs_end:
  f_close(&sd_file);
}