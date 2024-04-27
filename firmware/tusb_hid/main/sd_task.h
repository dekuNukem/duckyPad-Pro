#ifndef __SD_TASK_H
#define __SD_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>

#define SD_MOUNT_POINT "/sdcard"

#define SD_PIN_CLK 8
#define SD_PIN_CMD 9
#define SD_PIN_D0 7
#define SD_DETECT 6

uint8_t sd_init(void);

#ifdef __cplusplus
}
#endif

#endif


