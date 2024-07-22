set(requires espressif__esp_tinyusb fatfs esp_netif bt nvs_flash)

idf_component_register(
    SRCS "hid_task.c" "ds_vm.c" "keyboard.c" "keypress_task.c" "profiles.c" "rotary_encoder.c" "keypress_task.c" "ui_task.c" "ssd1306_fonts.c" "ssd1306.c" "sd_task.c" "main.c" "input_task.c" "neopixel_task.c" "profiles.c" "shared.c" "keyboard.c" "ds_vm.c" "hid_task.c" "bluetooth_task.c"
    INCLUDE_DIRS "."
    REQUIRES "${requires}"
    )
