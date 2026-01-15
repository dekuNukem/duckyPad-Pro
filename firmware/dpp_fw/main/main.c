#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "bootloader_random.h"
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
#include "mypwm.h"

/*
1.0.0
2024 11 18
Public release

1.0.1
2024 11 27
Fixed non-responsive mouse scroll bug

1.0.2
2024 12 13
added BT mode option: Auto/Always/Never

1.0.3
2024 12 18
fixed dead key behaviour

1.0.4
2025 01 06
Each profile now has their own keypress counter

1.0.5
2025 01 07
added _DP_MODEL reserved variable
fixed BT unpair typo
adjusted keypress counter increment logic

1.1.0
Persistent global variables $_GV0 to $_GV15
Waking up from sleep with +/- button no longer changes profile

1.1.1
Fixed `DP_SLEEP` wakes up right away

1.1.2
Assorted bugfix during backporting

1.1.3
fixed a bug of not saving settings

1.6.0
New profile folder name format

2.0.0
Apr 7 2025
Public beta release

2.0.1
Apr 14 2025
fixed MOUSE_MOVE not holding mouse button

2.0.2
Apr 24 2025
Reboots into MSC mode if no valid profile

2.1.0
May 24 2025
Caches recent keypresses
Reboots after changing Bluetooth mode

2.2.0
June 14 2025
Added option for rotary encoder half-step
increases sensitivity

2.3.0
June 29 2025
Increased persistent global variables to 32
$_GV0 to $_GV31
Read and write GVs via HID

2.3.2
July 8 2025
fixed a bug where HID buffer was not properly cleared

2.4.0
Nov 15 2025
Adding RTC support
    Setting / Reading RTC via HID commands
    Query time and date as reserved variables
    Detect if RTC is still valid

Print format reserved variables
    Signed / Unsigned / Hex upper case / Hex lower case
    Seletable precision

Anykey-to-Abort now works during DELAY command
Added bitwise XOR instruction in DSVM

Bug Fixs:
    Persistent state loading color of no-longer-assigned keys
    better handling of division by zero
    No longer shows stale content on OLED on reset

2.4.1
Nov 18 2025
Added _RTC_WDAY and _RTC_YDAY
Improved runtime error display

2.4.2
Nov 26 2025
More informative incompatible VM error screen

2.4.3
Dec 16 2025
Increased USB polling rate
Allows much faster typing

2.4.4
Dec 16 2025
Increased USB MSC buffer, faster transfer.

3.0.0
Dec 18 2025
Working on DSVM2
profile max len bug
New persistent state savefile format
Fixed user-defined key color saving

3.0.1
Dec 26 2025
Added PEEK8, POKE8, and scratch memory area
Added PUSH0 and DROP instruction

3.1.0
Dec 30 2025
the whole jan 2026 update
RANDINT()
PUTS()
RANDCHR()
Print format specifiers
PWMCTL
horizontal mouse scroll

3.2.0
Jan 11 2026
New OLED_PRINT instruction
Cleanup before public beta

3.3.1
Jan 15 2026
Added PEEK and POKE instructions
Added explict unsigned instructions
Cleaned up reserved variables
Memory-mapped IO
*/

uint8_t fw_version_major = 3;
uint8_t fw_version_minor = 3;
uint8_t fw_version_patch = 1;

uint8_t dsvm_version = 2;

static const char *TAG = "MAIN";

/*
    Notes:
    
    In menu config: tinyUSB FIFO size: 512 default, too slow. 2048 = 2.4x faster, 4096 = 3x

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
    neopixel_init();
    expansion_uart_init();
    is_rtc_valid = check_rtc_is_valid();

    if(sd_init())
    {
        oled_init();
        draw_nosd();
        idle_loop();
    }

    my_pwm_test();
    load_settings(&dp_settings);
    oled_init();
    if(dp_settings.bt_mode == BT_MODE_ALWAYS)
    {
        oled_say("BT Only Mode");
        delay_ms(1000);
    }
    else
    {
        oled_say("Booting...");
    }
    CLEAR_TEMP_BUF();
    snprintf(temp_buf, TEMP_BUFSIZE, "DP24_%02x%02x", esp_mac_addr[ESP_MAC_ADDR_SIZE-2], esp_mac_addr[ESP_MAC_ADDR_SIZE-1]);
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

    ensure_new_profile_format();
    
    if(scan_profiles() == PSCAN_ERROR_NO_PROFILE)
    {
        draw_noprofile();
        delay_ms(3000);
        generate_msc_flag_file();
        esp_restart();
    }

    load_keymap_by_name(dp_settings.current_kb_layout);

    if(dp_settings.bt_mode == BT_MODE_NEVER)
    {
        mount_hid_only();
        bootloader_random_enable();
    }
    else if(dp_settings.bt_mode == BT_MODE_ALWAYS)
    {
        my_bt_init();
    }
    else
    {
        mount_hid_only();
        bootloader_random_enable();
        if(wait_for_hid_connect(2000) == 0)
        {
            tinyusb_driver_uninstall();
            draw_no_usb_activity();
            bootloader_random_disable();
            delay_ms(2000);
            my_bt_init();
        }
    }

    profile_init();
    load_gv();
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