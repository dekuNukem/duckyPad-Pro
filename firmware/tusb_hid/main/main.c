#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"

#include "rotary_encoder.h"
#include "input_task.h"
#include "sd_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "shared.h"
#include "parser.h"
#include "ssd1306.h"

uint8_t fw_version_major = 0;
uint8_t fw_version_minor = 0;
uint8_t fw_version_patch = 1;

static const char *TAG = "MAIN";

/*
    xTaskCreate higher number = higher priority
    configMAX_PRIORITIES
*/

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(500)); // brief delay in case of SD card removal reboot
    gpio_install_isr_service(0); // BEFORE GPIO INIT

    my_rotary_encoder_init();
    switch_init();
    oled_init();
    if(sd_init())
    {
        print_nosd();
        while(1){vTaskDelay(pdMS_TO_TICKS(250));}
    }
    
    neopixel_init();
    xTaskCreate(kb_scan_task, "kb_scan_task", SW_SCAN_TASK_STACK_SIZE, NULL, 2, NULL);
    load_settings(&dp_settings);
    uint8_t pscan_result = scan_profiles();
    if(pscan_result == PSCAN_ERROR_NO_PROFILE)
    {
        print_noprofile();
        error_loop();
    }
    else if(pscan_result == PSCAN_ERROR_NO_MEMORY)
    {
        print_nomemory();
        error_loop();
    }

    goto_next_profile();
    
    while(1)
    {
        rotary_encoder_event_t event = { 0 };
	    if (xQueueReceive(rotary_encoder_event_queue, &event, 0) == pdTRUE)
        {
            printf("Event: id: %d pos: %ld, dir: %d\n", event.state.id, event.state.position, event.state.direction);
            if(event.state.direction == ROTARY_ENCODER_DIRECTION_COUNTER_CLOCKWISE)
                goto_next_profile();
            else if(event.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE)
                goto_prev_profile();
        }
        vTaskDelay(pdMS_TO_TICKS(33));
    }
}