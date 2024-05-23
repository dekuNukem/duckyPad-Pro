#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"

#include "input_task.h"
#include "sd_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "rotary_encoder.h"

static const char *TAG = "MAIN";


#define ROT_ENC_A_GPIO (RE1_A)
#define ROT_ENC_B_GPIO (RE1_B)

#define ENABLE_HALF_STEPS 0  // tracking at half step resolution, suitable for smooth encoder to make it more sensitive

void app_main(void)
{
    gpio_init();
    sd_init();
    oled_init();
    neopixel_init();

    // esp32-rotary-encoder requires that the GPIO ISR service is installed before calling rotary_encoder_register()
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // Initialise the rotary encoder device with the GPIOs for A and B signals
    rotary_encoder_info_t info = { 0 };
    ESP_ERROR_CHECK(rotary_encoder_init(&info, ROT_ENC_A_GPIO, ROT_ENC_B_GPIO));
    ESP_ERROR_CHECK(rotary_encoder_enable_half_steps(&info, ENABLE_HALF_STEPS));
    QueueHandle_t event_queue = rotary_encoder_create_queue();
    ESP_ERROR_CHECK(rotary_encoder_set_queue(&info, event_queue));

    while(1)
    {
        UBaseType_t items_in_queue = uxQueueMessagesWaiting(event_queue);
        printf("queue: %d\n", items_in_queue);
        // Wait for incoming events on the event queue.
        rotary_encoder_event_t event = { 0 };
        if (xQueueReceive(event_queue, &event, 0) == pdTRUE)
            printf("Event: position %ld, direction %d\n", event.state.position, event.state.direction);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}