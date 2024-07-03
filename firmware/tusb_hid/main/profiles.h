#ifndef __PROFILES_H
#define __PROFILES_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>
#include "input_task.h"
#include "neopixel_task.h"

#define TEMP_BUFSIZE 128
#define FILENAME_BUFSIZE 64
#define MAX_PROFILES 64
#define KEYNAME_SIZE 8

typedef struct
{
  uint32_t sleep_after_ms;
  uint8_t brightness_index;
  uint8_t orientation;
  char current_kb_layout[FILENAME_BUFSIZE];
} dp_global_settings;

/*
  this caches profile name, and printable keynames
*/
typedef struct
{
  uint8_t pf_number;
  uint8_t is_loaded;
  char dir_path[FILENAME_BUFSIZE];
  char* pf_name;
  char sw_name_firstline[TOTAL_OBSW_COUNT][KEYNAME_SIZE];
  char sw_name_secondline[TOTAL_OBSW_COUNT][KEYNAME_SIZE];
  uint8_t sw_color[MECH_OBSW_COUNT][THREE]; // only mechanical keys have RGB LEDs
  uint8_t sw_activation_color[MECH_OBSW_COUNT][THREE];
  uint8_t dim_unused_keys;
} profile_info;

extern profile_info all_profile_info[MAX_PROFILES];
extern dp_global_settings dp_settings;
extern uint8_t current_profile_number;
extern char temp_buf[TEMP_BUFSIZE];

uint8_t load_settings(dp_global_settings* dps);
uint8_t scan_profiles();
void error_loop(void);
void goto_next_profile(void);
void goto_prev_profile(void);
void profile_init(void);
void goto_profile(uint8_t profile_number);
uint8_t load_keymap_by_name(char* km_name);

extern const char* default_keymap_name;

#define PSCAN_OK 0
#define PSCAN_ERROR_NO_PROFILE 1

#ifdef __cplusplus
}
#endif

#endif


