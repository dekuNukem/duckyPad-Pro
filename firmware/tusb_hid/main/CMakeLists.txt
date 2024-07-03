idf_component_register(
    SRCS "keyboard.c" "keypress_task.c" "profiles.c" "rotary_encoder.c" "keypress_task.c" "ui_task.c" "ssd1306_fonts.c" "ssd1306.c" "sd_task.c" "main.c" "input_task.c" "neopixel_task.c" "profiles.c" "shared.c" "keyboard.c"
    INCLUDE_DIRS "."
    REQUIRES espressif__esp_tinyusb
    )
