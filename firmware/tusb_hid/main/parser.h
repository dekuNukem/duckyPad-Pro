#ifndef __PARSER_H
#define __PARSER_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>

#define TEMP_BUFSIZE 128
#define FILENAME_BUFSIZE 64

typedef struct
{
  uint32_t sleep_after_ms;
  uint8_t brightness_index;
  char current_kb_layout[FILENAME_BUFSIZE];
} dp_global_settings;

extern dp_global_settings dp_settings;


void load_settings(dp_global_settings* dps);


#ifdef __cplusplus
}
#endif

#endif


