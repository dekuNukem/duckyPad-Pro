idf_component_register(
    SRCS "rotary_encoder.c" "ui_task.c" "ssd1306_fonts.c" "ssd1306.c" "sd_task.c" "main.c" "input_task.c" "neopixel_task.c"
    INCLUDE_DIRS "."
    REQUIRES espressif__esp_tinyusb
    )
