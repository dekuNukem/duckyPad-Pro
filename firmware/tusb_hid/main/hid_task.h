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

#define HID_COMMAND_GET_INFO 0
#define HID_COMMAND_GOTO_PROFILE 1
#define HID_COMMAND_PREV_PROFILE 2
#define HID_COMMAND_NEXT_PROFILE 3

#define HID_COMMAND_SET_LED_SINGLE 4

#define HID_COMMAND_SW_RESET 20
#define HID_COMMAND_SLEEP 21
#define HID_COMMAND_WAKEUP 22

#define HID_RESPONSE_OK 0
#define HID_RESPONSE_GENERIC_ERROR 1
#define HID_RESPONSE_BUSY 2
// #define HID_RESPONSE_EOF 3
#define HID_RESPONSE_NO_PROFILE 4
#define HID_RESPONSE_INVALID_ARG 5
#define HID_RESPONSE_UNKNOWN_CMD 6

#define HID_USAGE_ID_KEYBOARD 1
#define HID_USAGE_ID_MEDIA_KEY 2
#define HID_USAGE_ID_MOUSE 3
#define HID_USAGE_ID_NAMED_PIPE 4

extern volatile uint8_t is_usb_hid_connected;
uint8_t wait_for_hid_connect(uint32_t how_long_ms);

#ifdef __cplusplus
}
#endif

#endif


