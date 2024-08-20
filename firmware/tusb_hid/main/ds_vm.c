#include <stdio.h>    /* printf */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "rotary_encoder.h"
#include "input_task.h"
#include "sd_task.h"
#include "ui_task.h"
#include "neopixel_task.h"
#include "shared.h"
#include "profiles.h"
#include "ssd1306.h"
#include "keypress_task.h"
#include "keyboard.h"
#include "ds_vm.h"

uint8_t bin_buf[BIN_BUF_SIZE];
uint8_t var_buf[VAR_BUF_SIZE];
uint16_t defaultdelay_value;
uint16_t defaultchardelay_value;
uint16_t charjitter_value;
uint32_t rand_min, rand_max;
uint16_t loop_size;
uint8_t epilogue_actions;
uint8_t allow_abort;
uint8_t key_press_count[MAX_TOTAL_SW_COUNT];

typedef struct
{
  uint8_t top;
  uint16_t stack[STACK_SIZE];
} my_stack;

my_stack arithmetic_stack, call_stack;

uint8_t read_byte(uint16_t addr)
{
  return bin_buf[addr];
}

void stack_init(my_stack* ms)
{
  ms->top = 0;
  memset(ms->stack, 0, STACK_SIZE*sizeof(uint16_t));
}

uint8_t stack_push(my_stack* ms, uint16_t value)
{
  if(ms->top >= STACK_SIZE)
    return EXE_STACK_OVERFLOW;
  ms->stack[ms->top] = value;
  ms->top++;
  return EXE_OK;
}

uint8_t stack_pop(my_stack* ms, uint16_t *result)
{
  if(ms->top == 0)
    return EXE_STACK_UNDERFLOW;
  ms->top--;
  *result = ms->stack[ms->top];
  ms->stack[ms->top] = 0;
  return EXE_OK;
}

uint16_t make_uint16(uint8_t b0, uint8_t b1)
{
  return b0 | (b1 << 8);
}

uint16_t binop_add(uint16_t a, uint16_t b) {return a + b;}
uint16_t binop_sub(uint16_t a, uint16_t b) {return a - b;}
uint16_t binop_mul(uint16_t a, uint16_t b) {return a * b;}
uint16_t binop_divide(uint16_t a, uint16_t b) {return a / b;}
uint16_t binop_mod(uint16_t a, uint16_t b) {return a % b;}
uint16_t binop_greater(uint16_t a, uint16_t b) {return a > b;}
uint16_t binop_greater_eq(uint16_t a, uint16_t b) {return a >= b;}
uint16_t binop_lower(uint16_t a, uint16_t b) {return a < b;}
uint16_t binop_lower_eq(uint16_t a, uint16_t b) {return a <= b;}
uint16_t binop_equal(uint16_t a, uint16_t b) {return a == b;}
uint16_t binop_not_equal(uint16_t a, uint16_t b) {return a != b;}
uint16_t binop_bitwise_and(uint16_t a, uint16_t b) {return a & b;}
uint16_t binop_bitwise_or(uint16_t a, uint16_t b) {return a | b;}
uint16_t binop_lshift(uint16_t a, uint16_t b) {return a << b;}
uint16_t binop_rshift(uint16_t a, uint16_t b) {return a >> b;}
uint16_t binop_logical_and(uint16_t a, uint16_t b) {return a && b;}
uint16_t binop_logical_or(uint16_t a, uint16_t b) {return a || b;}
uint16_t binop_power(uint16_t x, uint16_t exponent)
{
    uint16_t result = 1;
    for (int i = 0; i < exponent; ++i)
        result *= x;
    return result;
}

typedef uint16_t (*FUNC_PTR)(uint16_t, uint16_t);

void binop(ds3_exe_result* exe, FUNC_PTR bin_func)
{
  uint16_t rhs, lhs;
  uint8_t op_result = stack_pop(&arithmetic_stack, &rhs);
  if(op_result != EXE_OK)
  {
    exe->result = op_result;
    return;
  }
  op_result = stack_pop(&arithmetic_stack, &lhs);
  if(op_result != EXE_OK)
  {
    exe->result = op_result;
    return;
  }
  op_result = stack_push(&arithmetic_stack, bin_func(lhs, rhs));
  if(op_result != EXE_OK)
  {
    exe->result = op_result;
    return;
  }
}

#define VAR_BOUNDARY (0x1f)

void store_uint16_as_two_bytes_at(uint16_t addr, uint16_t value)
{
  var_buf[addr] = value & 0xff;
  var_buf[addr+1] = value >> 8;
}

void write_var(uint16_t addr, uint16_t value)
{
  if(addr == DEFAULTDELAY_ADDR)
    defaultdelay_value = value;
  else if (addr == DEFAULTCHARDELAY_ADDR)
    defaultchardelay_value = value;
  else if (addr == CHARJITTER_ADDR)
    charjitter_value = value;
  else if (addr == _RANDOM_MIN)
    rand_min = value;
  else if (addr == _RANDOM_MAX)
    rand_max = value;
  else if (addr == _RANDOM_INT)
    ; // this is read only, so do nothing
  else if (addr == _TIME_MS)
    ; // this is read only, so do nothing
  else if (addr == _TIME_S)
    ; // this is read only, so do nothing
  else if (addr == _READKEY)
    ; // this is read only, so do nothing
  else if (addr == _LOOP_SIZE)
    loop_size = value;
  else if (addr == _KEYPRESS_COUNT)
    ; // this is read only, so do nothing
  else if (addr == _NEEDS_EPILOGUE)
    epilogue_actions = value;
  else if (addr == _ALLOW_ABORT)
    allow_abort = value;
  else if(addr < VAR_BUF_SIZE)
    store_uint16_as_two_bytes_at(addr, value);
}

uint8_t get_first_active_key(void)
{
  switch_event_t sw_event = {0};
  if(xQueueReceive(switch_event_queue, &sw_event, 0) == pdFALSE)
    return 0;
  if(sw_event.type == SW_EVENT_SHORT_PRESS)
    return sw_event.id;
  return 0;
}

uint16_t read_var(uint16_t addr, uint8_t this_key_id)
{
  if(addr == DEFAULTDELAY_ADDR)
    return defaultdelay_value;
  else if (addr == DEFAULTCHARDELAY_ADDR)
    return defaultchardelay_value;
  else if (addr == CHARJITTER_ADDR)
    return charjitter_value;
  else if (addr == _RANDOM_MIN)
    return rand_min;
  else if (addr == _RANDOM_MAX)
    return rand_max;
  else if (addr == _RANDOM_INT)
    return rand() % (rand_max + 1 - rand_min) + rand_min;
  else if (addr == _TIME_MS)
    return (uint16_t)pdTICKS_TO_MS(xTaskGetTickCount());
  else if (addr == _TIME_S)
    return (uint16_t)(pdTICKS_TO_MS(xTaskGetTickCount()) / 1000);
  else if (addr == _LOOP_SIZE)
    return loop_size;
  else if (addr == _READKEY)
    return get_first_active_key();
  else if (addr == _KEYPRESS_COUNT)
    return key_press_count[this_key_id];
  else if (addr == _NEEDS_EPILOGUE)
    return epilogue_actions;
  else if (addr == _ALLOW_ABORT)
    return allow_abort;
  else if(addr < VAR_BUF_SIZE)
    return make_uint16(var_buf[addr], var_buf[addr+1]);
  return 0;
}

#define STR_BUF_SIZE 8
char make_str_buf[STR_BUF_SIZE];
#define READ_BUF_SIZE 256
char read_buffer[READ_BUF_SIZE];
char* make_str(uint16_t str_start_addr, uint8_t this_key_id)
{
  uint16_t curr_addr = str_start_addr;
  uint8_t this_char, lsb, msb;
  memset(read_buffer, 0, READ_BUF_SIZE);
  while(1)
  {
    this_char = read_byte(curr_addr);
    if(this_char == 0)
      break;

    if(this_char == VAR_BOUNDARY)
    {
      curr_addr++;
      lsb = read_byte(curr_addr);
      curr_addr++;
      msb = read_byte(curr_addr);
      curr_addr++;
      curr_addr++;
      uint16_t var_addr = make_uint16(lsb, msb);
      uint16_t var_value = read_var(var_addr, this_key_id);
      memset(make_str_buf, 0, STR_BUF_SIZE);
      sprintf(make_str_buf, "%d", var_value);
      strcat(read_buffer, make_str_buf);
      continue;
    }
    memset(make_str_buf, 0, STR_BUF_SIZE);
    sprintf(make_str_buf, "%c", this_char);
    strcat(read_buffer, make_str_buf);
    curr_addr++;
  }
  return read_buffer;
}

uint16_t this_index, red, green, blue;
void parse_swcc(uint8_t opcode, uint8_t key_id_0_indexed)
{
  stack_pop(&arithmetic_stack, &blue);
  stack_pop(&arithmetic_stack, &green);
  stack_pop(&arithmetic_stack, &red);
  stack_pop(&arithmetic_stack, &this_index);

  if(this_index == 0)
    this_index = key_id_0_indexed;
  else
    this_index--;
  if(this_index >= NEOPIXEL_COUNT)
    return;
  halt_all_animations();
  set_pixel_3color(this_index, red, green, blue);
  neopixel_draw_current_buffer();
}

void parse_swcf(void)
{
  stack_pop(&arithmetic_stack, &blue);
  stack_pop(&arithmetic_stack, &green);
  stack_pop(&arithmetic_stack, &red);
  halt_all_animations();
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
    set_pixel_3color(i, red, green, blue);
  neopixel_draw_current_buffer();
}

void parse_swcr(uint8_t key_id_0_indexed)
{
  uint16_t swcr_arg;
  stack_pop(&arithmetic_stack, &swcr_arg);

  if(swcr_arg == 0)
    swcr_arg = key_id_0_indexed;
  else
    swcr_arg--;

  if(swcr_arg >= NEOPIXEL_COUNT)
    redraw_bg(current_profile_number);
  else
    reset_key_color(swcr_arg);
}

void parse_olc(void)
{
  uint16_t xxx, yyy;
  stack_pop(&arithmetic_stack, &yyy);
  stack_pop(&arithmetic_stack, &xxx);
  if(xxx >= SSD1306_WIDTH || yyy >= SSD1306_HEIGHT)
    return;
  ssd1306_SetCursor(xxx, yyy);
}

void execute_instruction(uint16_t curr_pc, ds3_exe_result* exe, uint8_t this_key_id)
{
  uint8_t this_opcode = read_byte(curr_pc);
  uint8_t byte0 = read_byte(curr_pc+1);
  uint8_t byte1 = read_byte(curr_pc+2);
  uint8_t op_result;
  uint16_t op_data = make_uint16(byte0, byte1);
  // printf("PC: %04d | Opcode: %02d | 0x%02x 0x%02x | 0x%04x\n", curr_pc, this_opcode, byte0, byte1, op_data);
  
  exe->result = EXE_OK;
  exe->next_pc = curr_pc + INSTRUCTION_SIZE_BYTES;
  exe->data = 0;
  
  if(allow_abort && sw_queue_has_keydown_event())
  {
    exe->result = EXE_ABORTED;
    return;
  }
  else if(this_opcode == OP_NOP || this_opcode == OP_VMINFO)
  {
    return;
  }
  else if(this_opcode == OP_PUSHC)
  {
    op_result = stack_push(&arithmetic_stack, op_data);
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
  }
  else if(this_opcode == OP_PUSHV)
  {
    op_result = stack_push(&arithmetic_stack, read_var(op_data, this_key_id));
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
  }
  else if(this_opcode == OP_POP)
  {
    uint16_t this_item;
    op_result = stack_pop(&arithmetic_stack, &this_item);
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
    write_var(op_data, this_item);
  }
  else if(this_opcode == OP_BRZ)
  {
    uint16_t this_value;
    op_result = stack_pop(&arithmetic_stack, &this_value);
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
    if(this_value == 0)
      exe->next_pc = op_data;
  }
  else if(this_opcode == OP_JMP)
  {
    exe->next_pc = op_data;
  }
  else if(this_opcode == OP_CALL)
  {
    op_result = stack_push(&call_stack, exe->next_pc);
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
    exe->next_pc = op_data;
  }
  else if(this_opcode == OP_RET)
  {
    uint16_t return_pc;
    op_result = stack_pop(&call_stack, &return_pc);
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
    exe->next_pc = return_pc;
  }
  else if(this_opcode == OP_HALT)
  {
    exe->result = EXE_HALT;
  }
  else if(this_opcode == OP_EQ)
  {
    binop(exe, binop_equal);
  }
  else if(this_opcode == OP_NOTEQ)
  {
    binop(exe, binop_not_equal);
  }
  else if(this_opcode == OP_LT)
  {
    binop(exe, binop_lower);
  }
  else if(this_opcode == OP_LTE)
  {
    binop(exe, binop_lower_eq);
  }
  else if(this_opcode == OP_GT)
  {
    binop(exe, binop_greater);
  }
  else if(this_opcode == OP_GTE)
  {
    binop(exe, binop_greater_eq);
  }
  else if(this_opcode == OP_ADD)
  {
    binop(exe, binop_add);
  }
  else if(this_opcode == OP_SUB)
  {
    binop(exe, binop_sub);
  }
  else if(this_opcode == OP_MULT)
  {
    binop(exe, binop_mul);
  }
  else if(this_opcode == OP_DIV)
  {
    binop(exe, binop_divide);
  }
  else if(this_opcode == OP_MOD)
  {
    binop(exe, binop_mod);
  }
  else if(this_opcode == OP_POW)
  {
    binop(exe, binop_power);
  }
  else if(this_opcode == OP_LSHIFT)
  {
    binop(exe, binop_lshift);
  }
  else if(this_opcode == OP_RSHIFT)
  {
    binop(exe, binop_rshift);
  }
  else if(this_opcode == OP_BITOR)
  {
    binop(exe, binop_bitwise_or);
  }
  else if(this_opcode == OP_BITAND)
  {
    binop(exe, binop_bitwise_and);
  }
  else if(this_opcode == OP_LOGIOR)
  {
    binop(exe, binop_logical_or);
  }
  else if(this_opcode == OP_LOGIAND)
  {
    binop(exe, binop_logical_and);
  }
  else if(this_opcode == OP_STR || this_opcode == OP_STRLN)
  {
    char* str_buf = make_str(op_data, this_key_id);
    if(kb_print(str_buf, defaultchardelay_value, charjitter_value))
    {
      exe->result = EXE_ABORTED;
      return;
    }
    if(this_opcode == OP_STRLN)
    {
    	press_key(0x28, 0x03); // ENTER key
    	vTaskDelay(pdMS_TO_TICKS(defaultdelay_value));
    	release_key(0x28, 0x03);
    	vTaskDelay(pdMS_TO_TICKS(defaultdelay_value));
    }
  }
  else if(this_opcode == OP_KDOWN)
  {
  	press_key(byte0, byte1);
  	vTaskDelay(pdMS_TO_TICKS(defaultdelay_value));
  }
  else if(this_opcode == OP_KUP)
  {
  	release_key(byte0, byte1);
  	vTaskDelay(pdMS_TO_TICKS(defaultdelay_value));
  }
  else if(this_opcode == OP_MMOV)
  {
    my_key kk;
    kk.code = byte1;
    kk.code2 = byte0;
    kk.type = KEY_TYPE_MOUSE_MOVEMENT;
    keyboard_press(&kk, 0);
    vTaskDelay(pdMS_TO_TICKS(defaultdelay_value));
  }
  else if(this_opcode == OP_DELAY)
  {
    uint16_t delay_amount;
    op_result = stack_pop(&arithmetic_stack, &delay_amount);
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
    vTaskDelay(pdMS_TO_TICKS(delay_amount));
  }
  else if(this_opcode == OP_MSCL)
  {
    my_key kk;
    kk.code = byte0;
    kk.code2 = 0;
    kk.type = KEY_TYPE_MOUSE_WHEEL;
    keyboard_press(&kk, 0);
    vTaskDelay(pdMS_TO_TICKS(defaultdelay_value));
  }
  else if(this_opcode == OP_SWCC)
  {
    parse_swcc(this_opcode, this_key_id);
  }
  else if(this_opcode == OP_SWCF)
  {
    parse_swcf();
  }
  else if(this_opcode == OP_SWCR)
  {
    parse_swcr(this_key_id);
  }
  else if(this_opcode == OP_OLC)
  {
    parse_olc();
  }
  else if(this_opcode == OP_OLP)
  {
    char* str_buf = make_str(op_data, this_key_id);
    ssd1306_WriteString(str_buf, Font_7x10, White);
  }
  else if(this_opcode == OP_OLU)
  {
    ssd1306_UpdateScreen();
  }
  else if(this_opcode == OP_OLB)
  {
    ssd1306_Fill(Black);
  }
  else if(this_opcode == OP_OLR)
  {
    draw_profile(&all_profile_info[current_profile_number]);
  }
  else if(this_opcode == OP_BCLR)
  {
    clear_sw_re_queue();
  }
  else if(this_opcode == OP_NEXTP)
  {
    exe->result = EXE_ACTION_NEXT_PROFILE;
  }
  else if(this_opcode == OP_PREVP)
  {
    exe->result = EXE_ACTION_PREV_PROFILE;
  }
  else if(this_opcode == OP_SLEEP)
  {
    exe->result = EXE_ACTION_SLEEP;
  }
  else if(this_opcode == OP_GOTOP)
  {
    uint16_t target_profile;
    op_result = stack_pop(&arithmetic_stack, &target_profile);
    if(op_result != EXE_OK)
    {
      exe->result = op_result;
      return;
    }
    exe->result = EXE_ACTION_GOTO_PROFILE;
    exe->data = (uint8_t)target_profile;
  }
  else
  {
    exe->result = EXE_UNKNOWN_OPCODE;
  }
}

uint8_t load_dsb(char* dsb_path)
{
  FILE *sd_file = fopen(dsb_path, "r");
  if(sd_file == NULL)
    return EXE_DSB_FOPEN_FAIL;
  memset(bin_buf, 0, BIN_BUF_SIZE);
  if(fread(bin_buf, 1, BIN_BUF_SIZE, sd_file) == 0)
    return EXE_DSB_FREAD_ERROR;
  fclose(sd_file);
  if(bin_buf[0] != OP_VMINFO)
    return EXE_DSB_INCOMPATIBLE_VERSION;
  if(bin_buf[2] != dsvm_version)
    return EXE_DSB_INCOMPATIBLE_VERSION;
  return EXE_OK;
}

void run_dsb(ds3_exe_result* er, uint8_t this_key_id, char* dsb_path)
{
  uint8_t dsb_load_result = load_dsb(dsb_path);
  if(dsb_load_result)
  {
    er->result = dsb_load_result;
    return;
  }
  
  uint16_t current_pc = 0;
  stack_init(&arithmetic_stack);
  stack_init(&call_stack);
  defaultdelay_value = DEFAULT_CMD_DELAY_MS;
  defaultchardelay_value = DEFAULT_CHAR_DELAY_MS;
  charjitter_value = 0;
  rand_max = 65535;
  rand_min = 0;
  loop_size = 0;
  epilogue_actions = 0;
  allow_abort = 0;
  srand(xTaskGetTickCount());

  while(1)
  {
    execute_instruction(current_pc, er, this_key_id);
    if(er->result != EXE_OK)
      break;
    current_pc = er->next_pc;
  }
  er->epilogue_actions = epilogue_actions;
}
