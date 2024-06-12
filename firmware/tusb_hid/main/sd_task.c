#include "sd_task.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

const char *SD_TAG = "SD";

// make sure to enable long file name support in menuconfig!

sdmmc_card_t *my_sd_card;

uint8_t sd_init(void)
{
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = 2,
            .allocation_unit_size = 16 * 1024
        };
    const char mount_point[] = SD_MOUNT_POINT;
    ESP_LOGI(SD_TAG, "Initializing SD card");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = 80000; // 8 or 40MHz?
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1;
    slot_config.clk = SD_PIN_CLK;
    slot_config.cmd = SD_PIN_CMD;
    slot_config.d0 = SD_PIN_D0;
    ESP_LOGI(SD_TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &my_sd_card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(SD_TAG, "Failed to mount filesystem.");
        } else {
            ESP_LOGE(SD_TAG, "Failed to initialize the card");
        }
        return 1;
    }
    ESP_LOGI(SD_TAG, "Filesystem mounted");
    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, my_sd_card);
    return 0;
}
