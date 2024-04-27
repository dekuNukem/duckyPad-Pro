idf_component_register(
    SRCS "ui_task.c" "ssd1306_fonts.c" "ssd1306.c" "sd_task.c" "main.c" "input_task.c"
    INCLUDE_DIRS "."
    REQUIRES driver
    )
