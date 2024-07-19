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
#define HID_COMMAND_RELOAD_CURRENT_PROFILE 4
#define HID_COMMAND_SW_COLOR 5
#define HID_COMMAND_PRINT_TEXT 6
#define HID_COMMAND_PRINT_BITMAP 7
#define HID_COMMAND_CLEAR_SCREEN 8
#define HID_COMMAND_UPDATE_SCREEN 9
#define HID_COMMAND_LIST_FILES 10
#define HID_COMMAND_READ_FILE 11
#define HID_COMMAND_OP_RESUME 12
#define HID_COMMAND_OP_ABORT 13
#define HID_COMMAND_OPEN_FILE_FOR_WRITING 14
#define HID_COMMAND_WRITE_FILE 15
#define HID_COMMAND_CLOSE_FILE 16
#define HID_COMMAND_DELETE_FILE 17
#define HID_COMMAND_CREATE_DIR 18
#define HID_COMMAND_DELETE_DIR 19
#define HID_COMMAND_SW_RESET 20
#define HID_COMMAND_SLEEP 21


#define HID_RESPONSE_OK 0
#define HID_RESPONSE_ERROR 1
#define HID_RESPONSE_BUSY 2
#define HID_RESPONSE_EOF 3

#define HID_FILE_READ_BUF_SIZE 60

#define HID_USAGE_ID_KEYBOARD 1
#define HID_USAGE_ID_MEDIA_KEY 2
#define HID_USAGE_ID_MOUSE 3
#define HID_USAGE_ID_NAMED_PIPE 4

#ifdef __cplusplus
}
#endif

#endif


