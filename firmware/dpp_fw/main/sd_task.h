#ifndef __SD_TASK_H
#define __SD_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>

#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_vfs_fat.h"

#define SD_MOUNT_POINT "/sdcard"

#define SD_PIN_CLK 8
#define SD_PIN_CMD 9
#define SD_PIN_D0 7

uint8_t sd_init(void);

extern sdmmc_card_t *my_sd_card;

#ifdef __cplusplus
}
#endif

#endif


