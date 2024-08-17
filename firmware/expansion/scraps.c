uint8_t is_expansion_switch(uint8_t swid);
uint8_t is_onboard_mappable_switch(uint8_t swid);
uint8_t is_expansion_switch(uint8_t swid)
{
    return swid >= EXP_BUTTON_START && swid <= EXP_BUTTON_END;
}

uint8_t is_onboard_mappable_switch(uint8_t swid)
{
    return (swid >= MSW_0 && swid <= MAX_MSW) || swid == RE1_SW || swid == RE2_SW;
}
  printf("oosp %d %s %s\n", swid, press_path, release_path);

void draw_nodsb(uint8_t swid)
{
  ssd1306_Fill(Black);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "ERROR");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 0);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_Line(0,10,128,10,White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "File not found:");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 20);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "key%d.dsb", swid);
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 35);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Open & Save");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 70);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "on PC app");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 85);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "Press Any Key");
  ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 115);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

  ssd1306_UpdateScreen();
}


draw_nodsb
    printf("EXP: Ask start ID\n");
    // printf("radio: %s\n", expansion_rx_buf);
is_busy = 1;
      uint32_t start = pdTICKS_TO_MS(xTaskGetTickCount());
      handle_sw_event(&sw_event);
      printf("took %ldms\n", pdTICKS_TO_MS(xTaskGetTickCount())-start);
      is_busy = 0;
SW_EVENT_RELEASE
      printf("unsupported remote fw: %d\n", remote_fw_ver_major);
  
  printf("tdpR: %x\n", this_cmd);
      printf("Asking: %02x\n", cmd_ask_starting_id_towards_duckypad);
__disable_irq();
  softserial_putc(ch);
  __enable_irq();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // happens every 25ms
  // HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
  // channel_update(starting_id);
  printf("%d\n", micros());
}
// printf("channel %d pressed!\n", i);
// printf("channel %d released!\n", i);
// HAL_UART_Transmit(&away_from_duckypad_uart, (unsigned char *)&ch, 1, 100);
    else
    {
      // printf("READY. ID: %d\n", starting_id);
      HAL_Delay(500);
      uint8_t qsize = q_getCount(&switch_event_queue);
      if(qsize == 0)
        continue;
      printf("qsize: %d\n", qsize);
      uint8_t this_cmd;
      q_pop(&switch_event_queue, &this_cmd);
      printf("swe: %02x\n", this_cmd);
    }
		printf("rand: %d\n", get_rand_delay_ms());
    towards_duckypad_send(0xab);
    HAL_Delay(500);
void channel_update(void)
{
  uint8_t this_channel_status[CHANNEL_COUNT];

  this_channel_status[CHANNEL_1] = HAL_GPIO_ReadPin(CH1_GPIO_Port, CH1_Pin);
  this_channel_status[CHANNEL_2] = HAL_GPIO_ReadPin(CH2_GPIO_Port, CH2_Pin);
  this_channel_status[CHANNEL_3] = HAL_GPIO_ReadPin(CH3_GPIO_Port, CH3_Pin);
  this_channel_status[CHANNEL_4] = HAL_GPIO_ReadPin(CH4_GPIO_Port, CH4_Pin);
  this_channel_status[CHANNEL_5] = HAL_GPIO_ReadPin(CH5_GPIO_Port, CH5_Pin);
  this_channel_status[CHANNEL_6] = HAL_GPIO_ReadPin(CH6_GPIO_Port, CH6_Pin);

  for (int i = 0; i < CHANNEL_COUNT; ++i)
  {
    // if(button_status[i].prev_state == BUTTON_RELEASED && button_status[i].button_state == BUTTON_PRESSED)
    //   mark_as_pressed(i);
    // else if(button_status[i].prev_state == BUTTON_PRESSED && button_status[i].button_state == BUTTON_RELEASED)
    //   mark_as_released(i);
    // button_status[i].prev_state = button_status[i].button_state;
    printf("%d ", this_channel_status[i]);
  }
  printf("\n");
}