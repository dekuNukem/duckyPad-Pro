#include <stdio.h>
#include <string.h>
#include <stdint.h>

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
  uint8_t buffer[1024]; // Buffer to read file chunks
  size_t bytesRead;

  while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
  {
    crc = crc32(crc, buffer, bytesRead);
  }

  fclose(file);
  return crc;
}

int main(int argc, char *argv[])
{
  uint32_t result = calculate_crc32("DPP_FW_2.3.4.bin");
  printf("crc: 0x%x\n", result);
  return 0;
}

