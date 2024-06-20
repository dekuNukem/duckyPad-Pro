#ifndef __KEYPRESS_TASK_H
#define __KEYPRESS_TASK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <string.h>
#include "input_task.h"

#define KEYPRESS_TASK_STACK_SIZE 2048

void keypress_task(void *dummy);

#ifdef __cplusplus
}
#endif

#endif


