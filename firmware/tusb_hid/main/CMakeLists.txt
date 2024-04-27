idf_component_register(
    SRCS "oled_task.c" "sd_task.c" "main.c" "input_task.c"
    INCLUDE_DIRS "."
    REQUIRES driver
    )
