#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"

#include "rotary_encoder.h"
#include "input_task.h"
#include "sd_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "shared.h"
#include "profiles.h"
#include "ssd1306.h"
#include "esp_mac.h"
#include "keypress_task.h"
#include "unistd.h"
#include "keyboard.h"
#include "ds_vm.h"
#include "hid_task.h"

#include "tusb_msc_storage.h"
#include "diskio_impl.h"
#include "diskio_sdmmc.h"

#include "esp_mac.h"

#include <dirent.h> 
#include "bluetooth_task.h"


static const char *TAG = "USBHID";

volatile uint8_t is_usb_hid_connected;

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)
#define CUSTOM_HID_EPIN_SIZE 63
#define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE CUSTOM_HID_EPIN_SIZE

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

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    is_usb_hid_connected = 1;
    return hid_report_descriptor;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;
  return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    handle_hid_command(buffer, bufsize);
}

#define BT_HID_BUF_SIZE 6
uint8_t bt_hid_buf[BT_HID_BUF_SIZE];

void hid_send_bluetooth(uint8_t* usb_hid_buf)
{
    uint8_t hid_usage_type = usb_hid_buf[0];
    memset(bt_hid_buf, 0, BT_HID_BUF_SIZE);
    if(hid_usage_type == HID_USAGE_ID_KEYBOARD)
    {
        bt_hid_buf[0] = usb_hid_buf[1]; // modifier
        bt_hid_buf[1] = 0; // reserved
        bt_hid_buf[2] = usb_hid_buf[2];
        bt_hid_buf[3] = usb_hid_buf[3];
        bt_hid_buf[4] = usb_hid_buf[4];
        bt_hid_buf[5] = usb_hid_buf[5];
        ble_kb_send(bt_hid_buf);
    }
    else if(hid_usage_type == HID_USAGE_ID_MOUSE)
    {
        bt_hid_buf[0] = usb_hid_buf[1];
        bt_hid_buf[1] = usb_hid_buf[2];
        bt_hid_buf[2] = usb_hid_buf[3];
        bt_hid_buf[3] = usb_hid_buf[4];
        ble_mouse_send(bt_hid_buf);
    }
    else if(hid_usage_type == HID_USAGE_ID_MEDIA_KEY)
    {
        bt_hid_buf[0] = usb_hid_buf[1];
        bt_hid_buf[1] = usb_hid_buf[2];
        ble_mk_send(bt_hid_buf);
    }
}

#define USB_HID_BUF_SIZE 8
uint8_t esp_hid_buf[USB_HID_BUF_SIZE];
void hid_send_usb(uint8_t* usb_hid_buf)
{
    uint8_t hid_usage_type = usb_hid_buf[0];
    memset(esp_hid_buf, 0, USB_HID_BUF_SIZE);
    if(hid_usage_type == HID_USAGE_ID_KEYBOARD)
    {
        memcpy(esp_hid_buf, usb_hid_buf, BT_HID_BUF_SIZE);
        esp_hid_buf[0] = usb_hid_buf[1]; // modifier
        esp_hid_buf[1] = 0; // reserved
    }
    else if(hid_usage_type == HID_USAGE_ID_MOUSE)
    {
        memcpy(esp_hid_buf, usb_hid_buf+1, BT_HID_BUF_SIZE-1);
    }
    else if(hid_usage_type == HID_USAGE_ID_MEDIA_KEY)
    {
        esp_hid_buf[0] = usb_hid_buf[1];
        esp_hid_buf[1] = usb_hid_buf[2];
    }
    tud_hid_report(hid_usage_type, esp_hid_buf, sizeof(esp_hid_buf));
}

void USBD_CUSTOM_HID_SendReport(uint8_t* hid_buf)
{
    if(is_usb_hid_connected)
        hid_send_usb(hid_buf);
    else if(bluetooth_status == BT_CONNECTED)
        hid_send_bluetooth(hid_buf);
}

// ---------------- USB MSC --------------------

// #define USB_PID   0xd11c
#define USB_PID   (0xd11c+1)
#define USB_VID   0x0483
#define USB_BCD   0x0200

static tusb_desc_device_t msc_desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = USB_BCD,

    // As required by USB Specs IAD's subclass must be common class (2) and protocol must be IAD (1)
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,

    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

enum
{
  ITF_NUM_MSC = 0,
  ITF_NUM_HID,
  ITF_NUM_TOTAL
};

#define EPNUM_MSC_OUT     0x03
#define EPNUM_MSC_IN      0x83

#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

uint8_t const hid_msc_config_desc[] =
{
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),

    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, 5, EPNUM_MSC_OUT, EPNUM_MSC_IN, 64),
};

char *msc_hid_string_desc[] =
{
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},   // 0: is supported language is English (0x0409)
    "dekuNukem",            // 1: Manufacturer
    "duckyPad Pro",         // 2: Product
    "DP24_00000000",                // 3: Serial number
    "duckyPad Pro HID",              // 4
    "duckyPad Pro MSC",                     // 5
};

#define BASE_PATH "/data"

static void storage_mount_changed_cb(tinyusb_msc_event_t *event)
{
    printf("Storage mounted to application: %s", event->mount_changed_data.is_mounted ? "Yes" : "No");
}

void mount_usb_msc(void)
{
    sprintf(msc_hid_string_desc[3], "DP24_%02X%02X%02X%02X", esp_mac_addr[ESP_MAC_ADDR_SIZE-4], esp_mac_addr[ESP_MAC_ADDR_SIZE-3], esp_mac_addr[ESP_MAC_ADDR_SIZE-2], esp_mac_addr[ESP_MAC_ADDR_SIZE-1]);
    const tinyusb_msc_sdmmc_config_t config_sdmmc = {
        .card = my_sd_card,
        .callback_mount_changed = storage_mount_changed_cb,
        .mount_config.max_files = 5,
    };
    ESP_ERROR_CHECK(tinyusb_msc_storage_init_sdmmc(&config_sdmmc));
    ESP_ERROR_CHECK(tinyusb_msc_storage_mount(BASE_PATH));
    ESP_LOGI(TAG, "USB MSC initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &msc_desc_device,
        .string_descriptor = msc_hid_string_desc,
        .string_descriptor_count = sizeof(msc_hid_string_desc) / sizeof(msc_hid_string_desc[0]),
        .external_phy = false,
        .configuration_descriptor = hid_msc_config_desc,
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB MSC initialization DONE");
}

// ---------------------- USB MSC END -----------------

char* hid_string_descriptor[] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},   // 0: is supported language is English (0x0409)
    "dekuNukem",            // 1: Manufacturer
    "duckyPad Pro",         // 2: Product
    "DP24_00000000",        // 3: Serial number
    "duckyPad Pro HID",     // 4
};

static const uint8_t hid_only_config_desc[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, 0x00, 100),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

static tusb_desc_device_t hid_only_desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = USB_BCD,

    // As required by USB Specs IAD's subclass must be common class (2) and protocol must be IAD (1)
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,

    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

void mount_hid_only(void)
{
    sprintf(hid_string_descriptor[3], "DP24_%02X%02X%02X%02X", esp_mac_addr[ESP_MAC_ADDR_SIZE-4], esp_mac_addr[ESP_MAC_ADDR_SIZE-3], esp_mac_addr[ESP_MAC_ADDR_SIZE-2], esp_mac_addr[ESP_MAC_ADDR_SIZE-1]);
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &hid_only_desc_device,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_only_config_desc,
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB HID only initialization DONE");
}

// ----------------- HID command parsing -------------------

uint8_t hid_tx_buf[HID_TX_BUF_SIZE];

void send_hid_cmd_response(uint8_t* hid_cmdbuf)
{
    tud_hid_report(HID_USAGE_ID_NAMED_PIPE, hid_cmdbuf, HID_TX_BUF_SIZE-1);
}

void handle_hid_command(const uint8_t* hid_rx_buf, uint8_t rx_buf_size)
{
    uint8_t command_type = hid_rx_buf[1];

    memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
    hid_tx_buf[0] = 0;
    hid_tx_buf[1] = HID_RESPONSE_OK;

    /*
        duckyPad to PC
        [0]   seq number (not used)
        [1]   Status
    */
    if(is_busy)
    {
        hid_tx_buf[1] = HID_RESPONSE_BUSY;
        send_hid_cmd_response(hid_tx_buf);
        return;
    }

    if(is_profile_load_complete == 0 && (command_type != HID_COMMAND_GET_INFO && command_type != HID_COMMAND_SW_RESET))
    {
        hid_tx_buf[1] = HID_RESPONSE_NO_PROFILE;
        send_hid_cmd_response(hid_tx_buf);
        return;
    }

    /*
        GET INFO
        -----------
        PC to duckyPad:
        [0]   seq number (not used)
        [1]   command
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   Status, 0 = OK
        [2]   firmware version major
        [3]   firmware version minor
        [4]   firmware version patch
        [5]   hardware revision
        [6 - 9]   UUID (uint32_t)
        [10]   current profile
        [11] is_sleeping
    */
    if(command_type == HID_COMMAND_GET_INFO)
    {
        hid_tx_buf[2] = fw_version_major;
        hid_tx_buf[3] = fw_version_minor;
        hid_tx_buf[4] = fw_version_patch;
        hid_tx_buf[5] = 24;
        hid_tx_buf[6] = esp_mac_addr[2];
        hid_tx_buf[7] = esp_mac_addr[3];
        hid_tx_buf[8] = esp_mac_addr[4];
        hid_tx_buf[9] = esp_mac_addr[5];
        hid_tx_buf[10] = current_profile_number;
        hid_tx_buf[11] = is_sleeping;
        send_hid_cmd_response(hid_tx_buf);
    }
    /*
        GOTO PROFILE
        -----------
        PC to duckyPad:
        [0]   seq number (not used)
        [1]   command
        [2]   profile number to switch to
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   Status
    */
    else if(command_type == HID_COMMAND_GOTO_PROFILE)
    {
        uint8_t target_profile = hid_rx_buf[2];
        if(all_profile_info[target_profile].is_loaded)
        {
            wakeup_from_sleep_and_load_profile(target_profile);
            send_hid_cmd_response(hid_tx_buf);
        }
        else
        {
            hid_tx_buf[1] = HID_RESPONSE_INVALID_ARG;
            send_hid_cmd_response(hid_tx_buf);
        }
    }
    /*
        PREV PROFILE
        -----------
        PC to duckyPad:
        [0]   seq number
        [1]   command
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   0 = OK
    */
    else if(command_type == HID_COMMAND_PREV_PROFILE)
    {
        wakeup_from_sleep_no_load();
        goto_prev_profile();
        send_hid_cmd_response(hid_tx_buf);
    }
    /*
        NEXT PROFILE
        -----------
        PC to duckyPad:
        [0]   seq number
        [1]   command
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   0 = OK
    */
    else if(command_type == HID_COMMAND_NEXT_PROFILE)
    {
        wakeup_from_sleep_no_load();
        goto_next_profile();
        send_hid_cmd_response(hid_tx_buf);
    }
    /*
        SET LED SINGLE
        -----------
        PC to duckyPad:
        [0] seq number (not used)
        [1] command
        [2] LED index, 0 to 19
        [3] red
        [4] green
        [5] blue
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   0 = OK
    */
    else if(command_type == HID_COMMAND_SET_LED_SINGLE)
    {
        set_pixel_3color_update_buffer(hid_rx_buf[2], hid_rx_buf[3], hid_rx_buf[4], hid_rx_buf[5]);
        neopixel_draw_current_buffer();
        send_hid_cmd_response(hid_tx_buf);
    }
    /*
        SOFTWARE RESET
        -----------
        PC to duckyPad:
        [0]   seq number (not used)
        [1]   command
        [2]   reboot options. 0 = normal, 1 = reboot into MSC
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   0 = OK
    */
    else if(command_type == HID_COMMAND_SW_RESET)
    {
        send_hid_cmd_response(hid_tx_buf);
        if(hid_rx_buf[2])
            generate_msc_flag_file();
        esp_restart();
    }
    /*
        SLEEP
        -----------
        PC to duckyPad:
        [0]   seq number (not used)
        [1]   command
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   0 = OK
    */
    else if(command_type == HID_COMMAND_SLEEP)
    {
        send_hid_cmd_response(hid_tx_buf);
        start_sleeping();
    }
    /*
        WAKE UP
        -----------
        PC to duckyPad:
        [0]   seq number (not used)
        [1]   command
        -----------
        duckyPad to PC
        [0]   seq number (not used)
        [1]   0 = OK
    */
    else if(command_type == HID_COMMAND_WAKEUP)
    {
        wakeup_from_sleep_and_load_profile(current_profile_number);
        send_hid_cmd_response(hid_tx_buf);
    }
    else // not a valid HID command
    {
        hid_tx_buf[1] = HID_RESPONSE_UNKNOWN_CMD;
        send_hid_cmd_response(hid_tx_buf);
    }
}

uint8_t wait_for_hid_connect(uint32_t how_long_ms)
{
    uint32_t start_ts = pdTICKS_TO_MS(xTaskGetTickCount());
    while(1)
    {
        if(pdTICKS_TO_MS(xTaskGetTickCount()) - start_ts > how_long_ms)
            return 0;
        if(is_usb_hid_connected)
            return 1;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}