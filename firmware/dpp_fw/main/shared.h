#ifndef __SHARED_H
#define __SHARED_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp_ota_ops.h"

#define ESP_MAC_ADDR_SIZE 6

#define millis() pdTICKS_TO_MS(xTaskGetTickCount())

extern uint8_t esp_mac_addr[ESP_MAC_ADDR_SIZE];
extern uint8_t fw_version_major;
extern uint8_t fw_version_minor;
extern uint8_t fw_version_patch;
extern uint8_t dsvm_version;

void strip_newline(char* line, uint8_t size);
uint8_t how_many_digits(uint8_t number);
char* goto_next_arg(char* buf, char* buf_end);
void fw_update_check(void);
void delay_ms(uint32_t amount);

#define MY_UINT32_MAX 0xffffffff

#ifdef __cplusplus
}
#endif

#endif


