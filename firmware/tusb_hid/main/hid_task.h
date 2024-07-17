#ifndef __HID_TASK_H
#define __HID_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

void mount_usb_msc(void);
void app_send_hid_demo(void);
void mount_hid_only(void);
void USBD_CUSTOM_HID_SendReport(uint8_t* hid_buf, uint8_t hid_buf_size);

#ifdef __cplusplus
}
#endif

#endif


