set(srcs "esp_hid_device_main.c"
        "input_task.c"
         "esp_hid_gap.c")
set(include_dirs ".")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES esp_hid driver
                       PRIV_REQUIRES nvs_flash)
