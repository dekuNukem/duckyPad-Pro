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
#include "keyboard.h"
#include "hid_task.h"

#include "esp_vfs_fat.h"

uint8_t fw_version_major = 2;
uint8_t fw_version_minor = 3;
uint8_t fw_version_patch = 4;

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
    neopixel_init();
    if(sd_init())
    {
        draw_nosd();
        error_loop();
    }
    
    memset(temp_buf, 0, TEMP_BUFSIZE);
    sprintf(temp_buf, "DP24_%02x%02x", esp_mac_addr[ESP_MAC_ADDR_SIZE-2], esp_mac_addr[ESP_MAC_ADDR_SIZE-1]);
    f_setlabel(temp_buf);

    led_animation_init();

    xTaskCreate(kb_scan_task, "kb_scan_task", SW_SCAN_TASK_STACK_SIZE, NULL, 2, NULL);

    if(should_mount_usb_msc())
    {
        mount_usb_msc();
        draw_msc_mode();
        delete_msc_flag_file();
        error_loop();
    }
    
    load_settings(&dp_settings);
    uint8_t pscan_result = scan_profiles();
    if(pscan_result == PSCAN_ERROR_NO_PROFILE)
    {
        draw_noprofile();
        error_loop();
    }

    load_keymap_by_name(dp_settings.current_kb_layout);
    profile_init();
    is_profile_load_complete = 1;

    dp_settings.sleep_after_ms = 120000;
    xTaskCreate(keypress_task, "keypress_task", KEYPRESS_TASK_STACK_SIZE, NULL, 2, NULL);

    mount_hid_only();

    while(1)
    {
        led_animation_handler();

        uint32_t ms_since_last_keypress = pdTICKS_TO_MS(xTaskGetTickCount()) - last_keypress;

        if(is_busy == 0 && dp_settings.sleep_after_ms != 0 && ms_since_last_keypress > dp_settings.sleep_after_ms)
            start_sleeping();

        if(is_busy == 0 && ms_since_last_keypress > OLED_DIM_AFTER_MS)
            ssd1306_SetContrast(OLED_CONTRAST_DIM);

        vTaskDelay(pdMS_TO_TICKS(ANIMATION_FREQ_MS));
    }
}