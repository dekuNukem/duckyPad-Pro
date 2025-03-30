#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ds_vm.h>
#include "esp_log.h"
#include "profiles.h"
#include "shared.h"
#include "input_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "keyboard.h"
#include "ds_vm.h"
#include "keypress_task.h"

const char *OTA_TAG = "OTA";

uint8_t esp_mac_addr[ESP_MAC_ADDR_SIZE];

void strip_newline(char* line, uint16_t size)
{
  for(size_t i = 0; i < size; ++i)
    if(line[i] == '\n' || line[i] == '\r')
      line[i] = 0;
}

uint8_t how_many_digits(uint8_t number)
{
  if(number >= 100)
    return 3;
  if(number >= 10)
    return 2;
  return 1;
}

char* goto_next_arg(char* buf, char* buf_end)
{
  char* curr = buf;  
  if(buf == NULL || curr >= buf_end)
    return NULL;
  while(curr < buf_end && *curr != ' ')
      curr++;
  while(curr < buf_end && *curr == ' ')
      curr++;
  if(curr >= buf_end)
    return NULL;
  return curr;
}

const char* duckypad_pro_fw_filename_prefix = "DPP_FW_";

uint8_t is_fw_filename(const char* fw_str)
{
  size_t fw_name_len = strlen(fw_str);

  if (strncmp(fw_str, duckypad_pro_fw_filename_prefix, strlen(duckypad_pro_fw_filename_prefix)) != 0)
    return 0;

  if (fw_name_len < 4)
    return 0;

  if (strcmp(&fw_str[fw_name_len - 4], ".bin") == 0)
    return 1;
  return 0;
}

uint8_t find_firmware_file(char* name_buf, uint32_t name_buf_size)
{
  struct dirent *ent;

  DIR* dir = opendir("/sdcard");
  if (dir == NULL)
    return 0;

  // Loop through each file in the directory
  while((ent = readdir(dir)) != NULL)
  {
    if (ent->d_type == DT_REG && is_fw_filename(ent->d_name))
    {
      memset(name_buf, 0, name_buf_size);
      sprintf(name_buf, "/sdcard/%s", ent->d_name);
      return 1;
    }
  }

  closedir(dir);
  return 0; // No firmware file found
}

static uint32_t crc32(uint32_t crc, uint8_t *buf, size_t len)
{
  int k;
  crc = ~crc;
  while (len--)
  {
    crc ^= *buf++;
    for (k = 0; k < 8; k++)
      crc = crc & 1 ? (crc >> 1) ^ 0xedb88320 : crc >> 1;
  }
  return ~crc;
}

uint32_t calculate_crc32(const char *filename)
{
  FILE *file = fopen(filename, "rb");
  if(file == NULL)
    return 0;

  uint32_t crc = 0; // Initial CRC value
  uint8_t* crc_buf = bin_buf;
  size_t bytesRead;

  while ((bytesRead = fread(crc_buf, 1, BIN_BUF_SIZE, file)) > 0)
    crc = crc32(crc, crc_buf, bytesRead);

  fclose(file);
  memset(bin_buf, 0, BIN_BUF_SIZE);
  return crc;
}

typedef struct binary_data_t {
    uint32_t size;
    uint32_t remaining_size;
    void *data;
} binary_data_t;

size_t fpread(void *buffer, size_t size, size_t nitems, size_t offset, FILE *fp) {
  if (fseek(fp, offset, SEEK_SET) != 0)
    return 0;
  return fread(buffer, size, nitems, fp);
}

uint8_t update_firmware(const char* file_path)
{
    esp_ota_handle_t update_handle = 0;
    const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
    esp_err_t err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
    ESP_LOGI(OTA_TAG, "esp_begin result = %d", err);
    binary_data_t data;
    FILE *file = fopen(file_path, "rb");
    if(file == NULL)
    {
      ESP_LOGI(OTA_TAG, "UPDATE FILE NOT FOUND");
      return 1;
    }
    fseek(file, 0, SEEK_END);
    data.size = ftell(file);
    data.remaining_size = data.size;
    fseek(file, 0, SEEK_SET);
    ESP_LOGI(OTA_TAG, "size %lu", data.size);
    memset(bin_buf, 0, BIN_BUF_SIZE);
    data.data = bin_buf;
    while (data.remaining_size > 0)
    {
      size_t size = data.remaining_size <= BIN_BUF_SIZE ? data.remaining_size : BIN_BUF_SIZE;
      fpread(data.data, size, 1, data.size - data.remaining_size, file);
      err = esp_ota_write(update_handle, data.data, size);
      if (data.remaining_size <= BIN_BUF_SIZE)
        break;
      data.remaining_size -= BIN_BUF_SIZE;
    }
    ESP_LOGI(OTA_TAG, "Ota result = %d", err);
    err = esp_ota_end(update_handle);
    if (err != ESP_OK)
    {
      ESP_LOGE(OTA_TAG, "esp_ota_end failed: %d!", err);
      return 2;
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK)
    {
      ESP_LOGE(OTA_TAG, "esp_ota_set_boot_partition failed: %d!", err);
      return 3;
    }
    fclose(file);
    unlink(file_path);
    ESP_LOGI(OTA_TAG, "FW update success, rebooting!");
    return 0;
}

void fw_update_check(void)
{
  if(find_firmware_file(filename_buf, FILENAME_BUFSIZE) == 0)
    return;
  
  printf("FIRMWARE FOUND: %s\n", filename_buf);
  uint32_t crc32 = calculate_crc32(filename_buf);
  printf("CRC32: %lx\n", crc32);
  
  sprintf(temp_buf, "%lx", crc32);
  if(strstr(filename_buf, temp_buf) == NULL)
  {
    neopixel_fill(128, 0, 0);
    draw_fw_crc_error(filename_buf);
    block_until_anykey(SW_EVENT_SHORT_PRESS);
    goto_profile(current_profile_number);
    return;
  }

  neopixel_fill(64, 64, 64);
  draw_fw_update_ask(filename_buf);
  block_until_anykey(SW_EVENT_SHORT_PRESS);

  oled_say("Updating...");

  uint8_t update_result = update_firmware(filename_buf);
  if(update_result)
  {
    CLEAR_TEMP_BUF();
    sprintf(temp_buf, "Failed: %d", update_result);
    oled_say(temp_buf);
  }
  else
  {
    oled_say("Success!");
  }
  delay_ms(2000);
  esp_restart();
}

void delay_ms(uint32_t amount)
{
  if(amount == 0)
    return;
  vTaskDelay(pdMS_TO_TICKS(amount));
}