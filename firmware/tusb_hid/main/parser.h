#ifndef __PARSER_H
#define __PARSER_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>

#define TEMP_BUFSIZE 128
#define FILENAME_BUFSIZE TEMP_BUFSIZE
#define MAX_PROFILES 64

typedef struct
{
  uint32_t sleep_after_ms;
  uint8_t brightness_index;
  char current_kb_layout[FILENAME_BUFSIZE];
} dp_global_settings;

typedef struct
{
  uint8_t is_loaded;
  char name[FILENAME_BUFSIZE];
} profile_info;

extern profile_info current_profile_info;
extern dp_global_settings dp_settings;
extern uint8_t current_profile_index;

uint8_t load_settings(dp_global_settings* dps);
void mytest();
uint8_t scan_profiles();

#define PSCAN_OK 0
#define PSCAN_ERROR_NO_PROFILE 1
#define PSCAN_ERROR_NO_MEMORY 2

#ifdef __cplusplus
}
#endif

#endif


