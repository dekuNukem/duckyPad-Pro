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
void draw_nodsb(uint8_t swid);
void draw_msc_mode(void);
void draw_exe_error(uint8_t err_code);

#define OLED_DIM_AFTER_MS 60000
#define OLED_CONTRAST_DIM 100
#define OLED_CONTRAST_BRIGHT 255

#ifdef __cplusplus
}
#endif

#endif


