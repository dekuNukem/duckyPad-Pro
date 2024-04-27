#ifndef __OLED_TASK_H
#define __OLED_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "driver/spi_master.h"

#define OLED_MOSI 9
#define OLED_CS 12
#define OLED_DC 11
#define OLED_SCK 8

extern spi_device_handle_t my_spi_handle;

void oled_init(void);

#ifdef __cplusplus
}
#endif

#endif


