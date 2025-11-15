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
extern RTC_NOINIT_ATTR int16_t utc_offset_minutes;

void strip_newline(char* line, uint16_t size);
char* goto_next_arg(char* buf, char* buf_end);
void fw_update_check(void);
void delay_ms(uint32_t amount);
uint32_t bytes_to_uint32_big_endian(const uint8_t b[4]);
uint8_t check_rtc_is_valid(void);
void split_uint32_be(uint32_t value, uint8_t *msb, uint8_t *b1, uint8_t *b2, uint8_t *lsb);

#define MY_UINT32_MAX 0xffffffff

#ifdef __cplusplus
}
#endif

#endif


