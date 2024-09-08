#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
  uint16_t test = 65535;
  printf("result: %d\n", (int16_t)test*2);
}

