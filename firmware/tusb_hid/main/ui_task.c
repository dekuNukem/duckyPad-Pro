#include "ui_task.h"
#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "UI";
spi_device_handle_t my_spi_handle;

void oled_init(void)
{
    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = OLED_SCK,
        .mosi_io_num = OLED_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SSD1306_WIDTH*SSD1306_HEIGHT/8,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 500000,
        .mode = 0,
        .queue_size = 1,
        .spics_io_num = OLED_CS,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &my_spi_handle));
}

char* my_data = "hello";
spi_transaction_t my_transaction;

void write_test(void)
{
    ESP_ERROR_CHECK(spi_device_acquire_bus(my_spi_handle, portMAX_DELAY));
    uint8_t tx_size_bytes = strlen(my_data);

    memset(&my_transaction, 0, sizeof(my_transaction));
    my_transaction.length = tx_size_bytes * 8;
    // my_transaction.flags = SPI_TRANS_USE_TXDATA;
    my_transaction.tx_buffer = my_data;

    ESP_ERROR_CHECK(spi_device_polling_transmit(my_spi_handle, &my_transaction));
    spi_device_release_bus(my_spi_handle);
}