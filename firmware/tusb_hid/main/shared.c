#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ds_vm.h>
#include "shared.h"

uint8_t esp_mac_addr[ESP_MAC_ADDR_SIZE];

void strip_newline(char* line, uint8_t size)
{
  for(int i = 0; i < size; ++i)
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