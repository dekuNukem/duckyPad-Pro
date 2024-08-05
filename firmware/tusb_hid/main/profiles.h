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
  uint8_t brightness_index;
  uint8_t sleep_index;
  // uint8_t screen_orientation;
  uint8_t bluetooth_priority;
  uint8_t last_used_profile;
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
extern char filename_buf[FILENAME_BUFSIZE];

uint8_t load_settings(dp_global_settings* dps);
uint8_t scan_profiles();
void error_loop(void);
void goto_next_profile(void);
void goto_prev_profile(void);
void profile_init(void);
void goto_profile(uint8_t profile_number);
uint8_t load_keymap_by_name(char* km_name);
uint8_t get_next_keymap(char* current_keymap_filename, char* next_keymap_filename);
uint8_t get_first_keymap(char* keymap_filename);
uint8_t save_settings(dp_global_settings* dps);
uint8_t should_mount_usb_msc(void);
void delete_msc_flag_file(void);
void generate_msc_flag_file(void);
void save_persistent_state(uint8_t epilogue_value, uint8_t swid);
uint8_t goto_profile_without_updating_rgb_LED(uint8_t profile_number);
uint8_t load_persistent_state(void);

extern uint8_t is_profile_load_complete;

#define PSCAN_OK 0
#define PSCAN_ERROR_NO_PROFILE 1

#define ERROR_NO_KEYMAP_FOLDER 1
#define ERROR_KEYMAP_NOT_FOUND 2

#ifdef __cplusplus
}
#endif

#endif


