#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"
#include "tusb_msc_storage.h"
#include "diskio_impl.h"
#include "diskio_sdmmc.h"
#include "sd_task.h"

#define APP_BUTTON (GPIO_NUM_13) // Use BOOT signal by default
static const char *TAG = "example";

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)
#define CUSTOM_HID_EPIN_SIZE 63
#define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE CUSTOM_HID_EPIN_SIZE
/**
 * @brief HID report descriptor
 *
 * In this example we implement Keyboard + Mouse HID device,
 * so we must define both report descriptors
 */
const uint8_t hid_report_descriptor[] = {
  // Report ID 1: Keyboard
  0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
  0x09, 0x06,        // Usage (Keyboard)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x19, 0xE0,        //   Usage Minimum (0xE0)
  0x29, 0xE7,        //   Usage Maximum (0xE7)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x95, 0x06,        //   Report Count (3)
  0x75, 0x08,        //   Report Size (8)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x94,        //   Logical Maximum (0x94) originally 65, 73 supports F13 - F24, 94 for japanese and korean language keys
  0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
  0x19, 0x00,        //   Usage Minimum (0x00)
  0x29, 0x94,        //   Usage Maximum (0x94) originally 65, 73 supports F13 - F24
  0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              // End Collection
  // Report ID 2: Media Keys
  0x05, 0x0C,        // Usage Page (Consumer)
  0x09, 0x01,        // Usage (Consumer Control)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x02,        //   Report ID (2)
  0x05, 0x0C,        //   Usage Page (Consumer)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x08,        //   Report Count (8)
  0x09, 0xB5,        //   Usage (Scan Next Track)
  0x09, 0xB6,        //   Usage (Scan Previous Track)
  0x09, 0xB7,        //   Usage (Stop)
  0x09, 0xB8,        //   Usage (Eject)
  0x09, 0xCD,        //   Usage (Play/Pause)
  0x09, 0xE2,        //   Usage (Mute)
  0x09, 0xE9,        //   Usage (Volume Increment)
  0x09, 0xEA,        //   Usage (Volume Decrement)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,               // End Collection
  // Report ID 3: Mouse
  0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
  0x09, 0x02,        // Usage (Mouse)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x03,        //   Report ID (3)
  0x09, 0x01,        //   Usage (Pointer)
  0xA1, 0x00,        //   Collection (Physical)
  0x05, 0x09,        //     Usage Page (Button)
  0x19, 0x01,        //     Usage Minimum (0x01)
  0x29, 0x03,        //     Usage Maximum (0x03)
  0x15, 0x00,        //     Logical Minimum (0)
  0x25, 0x01,        //     Logical Maximum (1)
  0x95, 0x03,        //     Report Count (3)
  0x75, 0x01,        //     Report Size (1)
  0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x95, 0x01,        //     Report Count (1)
  0x75, 0x05,        //     Report Size (5)
  0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
  0x09, 0x30,        //     Usage (X)
  0x09, 0x31,        //     Usage (Y)
  0x09, 0x38,        //     Usage (Wheel)
  0x15, 0x81,        //     Logical Minimum (-127)
  0x25, 0x7F,        //     Logical Maximum (127)
  0x75, 0x08,        //     Report Size (8)
  0x95, 0x03,        //     Report Count (3)
  0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              //   End Collection
  0x09, 0x3C,        //   Usage (Motion Wakeup)
  0x05, 0xFF,        //   Usage Page (Reserved 0xFF)
  0x09, 0x01,        //   Usage (0x01)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x02,        //   Report Count (2)
  0xB1, 0x22,        //   Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x06,        //   Report Size (6)
  0x95, 0x01,        //   Report Count (1)
  0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0,              // End Collection
  // Report ID 4: Custom HID communication pipe
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x3A,                    // USAGE (keyboard?)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
  // IN report
  0x85, 0x04,                    //   REPORT_ID (4)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x95, CUSTOM_HID_EPIN_SIZE,        //   REPORT_COUNT (this is the byte length)
  0x09, 0x3A,                    //   USAGE (keyboard?)
  0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
  // OUT report
  0x85, 0x05,                    //   REPORT_ID (5)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x95, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE,       //   REPORT_COUNT (this is the byte length)
  0x09, 0x3A,                    //   USAGE (keyboard?)
  0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
  0xc0                           // END_COLLECTION
}; 

/**
 * @brief String descriptor
 */
const char* my_usb_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "dekuNukem",             // 1: Manufacturer
    "duckyPad2",      // 2: Product
    "80085",              // 3: Serials, should use chip ID
    "duckyPad_HID",  // 4: HID
};

/**
 * @brief Configuration descriptor
 *
 * This is a simple configuration descriptor that defines 1 configuration and 1 HID interface
 */
static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

/********* TinyUSB HID callbacks ***************/

// Invoked when received GET HID REPORT DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

uint8_t hid_out_buf[CUSTOM_HID_EPIN_SIZE];
uint8_t needs_respond;

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    printf("\n\ngot data!\n");
    printf("%d %d %d %d\n", instance, report_id, report_type, bufsize);
    for (size_t i = 0; i < bufsize; i++)
        printf("%d ", buffer[i]);
    printf("\n\n");
    needs_respond = 1;
}

/********* Application ***************/

static void app_send_hid_demo(void)
{

    /*
    tud_hid_report, first argument is usage ID, see HID descriptor, keyboard is 1, media
    key is 2, mouse is 3, etc

    can also use tud_hid_keyboard_report, but probably easier to use tud_hid_report
    */
    // Keyboard output: Send key 'a/A' pressed and released
    ESP_LOGI(TAG, "Sending Keyboard report");
    uint8_t keycode[6] = {0x40, 0, 0, 0, 0, 0};
    tud_hid_report(2, keycode, 6);
    vTaskDelay(pdMS_TO_TICKS(100));
    memset(keycode, 0, 6);
    tud_hid_report(2, keycode, 6);

    // Mouse output: Move mouse cursor in square trajectory
    // ESP_LOGI(TAG, "Sending Mouse report");
    // for (size_t i = 0; i < 10; i++)
    // {
    //     tud_hid_mouse_report(3, 0x00, 5, 5, 0, 0);
    //     vTaskDelay(pdMS_TO_TICKS(20));
    // }
}

// ---------------- USB MSC --------------------

enum {
    ITF_NUM_MSC = 0,
    ITF_NUM_TOTAL
};

enum {
    EDPT_CTRL_OUT = 0x00,
    EDPT_CTRL_IN  = 0x80,

    EDPT_MSC_OUT  = 0x01,
    EDPT_MSC_IN   = 0x81,
};


static tusb_desc_device_t descriptor_config = {
    .bLength = sizeof(descriptor_config),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A, // This is Espressif VID. This needs to be changed according to Users / Customers
    .idProduct = 0x4002,
    .bcdDevice = 0x100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

static uint8_t const msc_fs_configuration_desc[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(0, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

    // something wrong with this:
    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 0, EDPT_MSC_OUT, EDPT_MSC_IN, 64),
};

static char const *string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 },  // 0: is supported language is English (0x0409)
    "TinyUSB",                      // 1: Manufacturer
    "TinyUSB Device",               // 2: Product
    "123456",                       // 3: Serials
    "Example MSC",                  // 4. MSC
};

#define BASE_PATH "/data" // base path to mount the partition


static void storage_mount_changed_cb(tinyusb_msc_event_t *event)
{
    ESP_LOGI(TAG, "Storage mounted to application: %s", event->mount_changed_data.is_mounted ? "Yes" : "No");
}
// ---------------------- USB MSC END -----------------

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing storage...");
    if(sd_init())
        goto main_loop;

    const tinyusb_msc_sdmmc_config_t config_sdmmc = {
        .card = my_sd_card,
        .callback_mount_changed = storage_mount_changed_cb,
        .mount_config.max_files = 5,
    };
    ESP_ERROR_CHECK(tinyusb_msc_storage_init_sdmmc(&config_sdmmc));
    ESP_ERROR_CHECK(tinyusb_msc_storage_mount(BASE_PATH));

    ESP_LOGI(TAG, "USB MSC initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &descriptor_config,
        .string_descriptor = string_desc_arr,
        .string_descriptor_count = sizeof(string_desc_arr) / sizeof(string_desc_arr[0]),
        .external_phy = false,
        .configuration_descriptor = msc_fs_configuration_desc,
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB MSC initialization DONE");

    main_loop:
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}


// void app_main(void)
// {
//     // Initialize button that will trigger HID reports
//     const gpio_config_t boot_button_config = {
//         .pin_bit_mask = BIT64(APP_BUTTON),
//         .mode = GPIO_MODE_INPUT,
//         .intr_type = GPIO_INTR_DISABLE,
//         .pull_up_en = true,
//         .pull_down_en = false,
//     };
//     ESP_ERROR_CHECK(gpio_config(&boot_button_config));

//     ESP_LOGI(TAG, "USB initialization");
//     const tinyusb_config_t tusb_cfg = {
//         .device_descriptor = NULL,
//         .string_descriptor = my_usb_string_descriptor,
//         .string_descriptor_count = sizeof(my_usb_string_descriptor) / sizeof(my_usb_string_descriptor[0]),
//         .external_phy = false,
//         .configuration_descriptor = hid_configuration_descriptor,
//     };

//     ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
//     ESP_LOGI(TAG, "USB initialization DONE");

//     while (1)
//     {
//         if (tud_mounted()) 
//         {
//             static bool send_hid_data = false;
//             if (send_hid_data)
//                 app_send_hid_demo();
//             send_hid_data = !gpio_get_level(APP_BUTTON);
//         }

//         if(needs_respond)
//         {
//             printf("responding!\n");
//             tud_hid_report(4, hid_out_buf, CUSTOM_HID_EPIN_SIZE);
//             needs_respond = 0;
//         }
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }
