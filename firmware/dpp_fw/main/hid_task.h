#ifndef __HID_TASK_H
#define __HID_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

void mount_usb_msc(void);
void mount_hid_only(void);
void USBD_CUSTOM_HID_SendReport(uint8_t* hid_buf);
void handle_hid_command(const uint8_t* hid_rx_buf, uint8_t rx_buf_size);

#define HID_COMMAND_GET_INFO               0x00
#define HID_COMMAND_GOTO_PROFILE_BY_NUMBER 0x01
#define HID_COMMAND_PREV_PROFILE           0x02
#define HID_COMMAND_NEXT_PROFILE           0x03
#define HID_COMMAND_SET_LED_SINGLE         0x04
#define HID_COMMAND_SW_RESET               0x14
#define HID_COMMAND_SLEEP                  0x15
#define HID_COMMAND_WAKEUP                 0x16
#define HID_COMMAND_GOTO_PROFILE_BY_NAME   0x17
#define HID_COMMAND_DUMP_GV                0x18
#define HID_COMMAND_WRITE_GV               0x19
#define HID_COMMAND_SET_RTC                0x1A

#define HID_RESPONSE_OK 0
#define HID_RESPONSE_GENERIC_ERROR 1
#define HID_RESPONSE_BUSY 2

#define HID_RESPONSE_NO_PROFILE 4
#define HID_RESPONSE_INVALID_ARG 5
#define HID_RESPONSE_UNKNOWN_CMD 6

#define HID_USAGE_ID_KEYBOARD 1
#define HID_USAGE_ID_MEDIA_KEY 2
#define HID_USAGE_ID_MOUSE 3
#define HID_USAGE_ID_NAMED_PIPE 4

extern volatile uint8_t is_usb_hid_connected;
uint8_t wait_for_hid_connect(uint32_t how_long_ms);
extern volatile uint8_t needs_gv_save;
extern volatile uint8_t is_rtc_valid;

#ifdef __cplusplus
}
#endif

#endif


