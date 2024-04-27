idf_component_register(
    SRCS "helpers.c" "input_task.c" "tusb_hid_example_main.c"
    INCLUDE_DIRS "."
    REQUIRES driver
    )
