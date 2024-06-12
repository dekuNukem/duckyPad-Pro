#ifndef __SHARED_H
#define __SHARED_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>

extern uint8_t fw_version_major;
extern uint8_t fw_version_minor;
extern uint8_t fw_version_patch;

void strip_newline(char* line, uint8_t size);
uint8_t how_many_digits(uint8_t number);


#ifdef __cplusplus
}
#endif

#endif


