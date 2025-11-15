#ifndef __OLED_TASK_H
#define __OLED_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "driver/spi_master.h"
#include "profiles.h"

#define OLED_MOSI 10
#define OLED_CS 14
#define OLED_DC 12
#define OLED_SCK 11
#define OLED_RESET 13

extern spi_device_handle_t my_spi_handle;

void oled_init(void);
void draw_nosd(void);
void draw_noprofile(void);
void draw_profile(profile_info* this_profile);
void draw_settings(dp_global_settings *dps);
void draw_msc_mode(void);
void draw_exe_error(uint8_t err_code);
void draw_fw_update_ask(char* fw_path_str);
void draw_fw_crc_error(char* fw_path_str);
void oled_say(char* what);
void update_status_icon();
void draw_status_icon(uint8_t origx, uint8_t origy, uint8_t update_screen);
void draw_bt_pin(uint32_t this_bt_pin);
void draw_nvm_erase();
void draw_no_usb_activity();
void update_kbled_icon(uint8_t this_led_state);
void draw_kbled_icon(uint8_t this_led_state, uint8_t update_screen);

#define ONE_MINUTE_IN_MS (60*1000)
#define ONE_HOUR_IN_MS (60*ONE_MINUTE_IN_MS)
#define DONT_SLEEP 0xffffffff
#define SLEEP_OPTION_SIZE 7

#define OLED_DIM_AFTER_MS ONE_MINUTE_IN_MS
#define OLED_CONTRAST_DIM 100
#define OLED_CONTRAST_BRIGHT 255

#define SETTINGS_ENTRY_SIZE 6

extern uint32_t sleep_after_ms_index_to_time_lookup[SLEEP_OPTION_SIZE];

#ifdef __cplusplus
}
#endif

#endif


