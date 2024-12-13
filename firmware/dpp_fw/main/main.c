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
#include "bluetooth_task.h"
#include "esp_vfs_fat.h"
#include "ds_vm.h"

/*
1.0.0
2024 11 18
Public release

1.0.1
2024 11 27
Fixed non-responsive mouse scroll bug

1.6.0
2024 12 13
added BT mode option: Auto/Always/Never
*/

uint8_t fw_version_major = 1;
uint8_t fw_version_minor = 6;
uint8_t fw_version_patch = 0;
uint8_t dsvm_version = 1;

static const char *TAG = "MAIN";

/*
    xTaskCreate higher number = higher priority
    configMAX_PRIORITIES
*/

void app_main(void)
{
    // brief delay in case of SD card removal reboot
    delay_ms(250);
    gpio_install_isr_service(0); // BEFORE GPIO INIT
    esp_read_mac(esp_mac_addr, ESP_MAC_ADDR_SIZE);
    my_rotary_encoder_init();
    switch_init();
    oled_init();
    neopixel_init();
    expansion_uart_init();
    oled_say("Booting...");
    if(sd_init())
    {
        draw_nosd();
        idle_loop();
    }
    
    memset(temp_buf, 0, TEMP_BUFSIZE);
    sprintf(temp_buf, "DP24_%02x%02x", esp_mac_addr[ESP_MAC_ADDR_SIZE-2], esp_mac_addr[ESP_MAC_ADDR_SIZE-1]);
    f_setlabel(temp_buf);

    led_animation_init();

    xTaskCreate(kb_scan_task, "kb_scan_task", SW_SCAN_TASK_STACK_SIZE, NULL, 5, NULL);

    if(should_mount_usb_msc())
    {
        mount_usb_msc();
        delete_msc_flag_file();
        neopixel_fill(32,32,32);
        draw_msc_mode();
        block_until_plus_minus_long_press();
        delay_ms(500);
        esp_restart();
    }

    is_busy = 1;
    fw_update_check();
    esp_ota_mark_app_valid_cancel_rollback();
    is_busy = 0;
    
    load_settings(&dp_settings);
    if(scan_profiles() == PSCAN_ERROR_NO_PROFILE)
    {
        draw_noprofile();
        idle_loop();
    }

    load_keymap_by_name(dp_settings.current_kb_layout);

    mount_hid_only();
    if(wait_for_hid_connect(1500) == 0)
    {
        tinyusb_driver_uninstall();
        draw_no_usb_activity();
        delay_ms(2000);
        my_bt_init();
        goto_profile(current_profile_number);
    }

    profile_init();
    is_profile_load_complete = 1;

    xTaskCreate(keypress_task, "keypress_task", KEYPRESS_TASK_STACK_SIZE, NULL, 6, NULL);

    while(1)
    {
        led_animation_handler();
        ssd1306_SetContrast(oled_brightness);

        uint32_t ms_since_last_keypress = pdTICKS_TO_MS(xTaskGetTickCount()) - last_keypress;

        delay_ms(ANIMATION_FREQ_MS);

        if(is_busy)
            continue;

        if(ms_since_last_keypress > sleep_after_ms_index_to_time_lookup[dp_settings.sleep_index])
            start_sleeping();

        if(ms_since_last_keypress > OLED_DIM_AFTER_MS)
            oled_brightness = OLED_CONTRAST_DIM;
    }
}