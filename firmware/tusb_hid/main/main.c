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
#include "profiles.h"
#include "ssd1306.h"
#include "esp_mac.h"
#include "keypress_task.h"

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
    esp_read_mac(esp_mac_addr, ESP_MAC_ADDR_SIZE);
    my_rotary_encoder_init();
    switch_init();
    oled_init();
    if(sd_init())
    {
        print_nosd();
        while(1){vTaskDelay(pdMS_TO_TICKS(250));}
    }
    
    neopixel_init();
    led_animation_init();

    xTaskCreate(kb_scan_task, "kb_scan_task", SW_SCAN_TASK_STACK_SIZE, NULL, 2, NULL);
    load_settings(&dp_settings);
    uint8_t pscan_result = scan_profiles();
    if(pscan_result == PSCAN_ERROR_NO_PROFILE)
    {
        print_noprofile();
        error_loop();
    }

    profile_init();

    while(1)
    {
        rotary_encoder_event_t re_event = { 0 };
	    if (xQueueReceive(rotary_encoder_event_queue, &re_event, 0) == pdTRUE)
        {
            printf("Event: id: %d pos: %ld, dir: %d\n", re_event.state.id, re_event.state.position, re_event.state.direction);
            if(re_event.state.direction == ROTARY_ENCODER_DIRECTION_COUNTER_CLOCKWISE)
                goto_next_profile();
            else if(re_event.state.direction == ROTARY_ENCODER_DIRECTION_CLOCKWISE)
                goto_prev_profile();
        }

        switch_event_t sw_event = { 0 };
        if (xQueueReceive(switch_event_queue, &sw_event, 0) == pdTRUE)
        {
            printf("id: %d lvl: %d\n", sw_event.id, sw_event.level);
            if(sw_event.level)
                play_keydown_animation(current_profile_number, sw_event.id);
            else
                play_keyup_animation(current_profile_number, sw_event.id);
        }
        led_animation_handler();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}