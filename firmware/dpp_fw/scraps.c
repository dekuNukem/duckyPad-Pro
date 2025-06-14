void goto_profile(uint8_t profile_number)
{
  if(goto_profile_without_updating_rgb_LED(profile_number))
    return;
  if(load_persistent_state())
    redraw_bg(profile_number);
  else
    neopixel_draw_current_buffer();
  if(profile_number > 2)
    set_re_halfstep(1, 1);
  else
    set_re_halfstep(1, 0);
}

printf("%s:\n", all_profile_info[1].pf_name);
  for(size_t i = 0; i < TOTAL_OBSW_COUNT; i++)
  {
    char* tttt = all_profile_info[1].sw_name_firstline[i];
    printf("%d %s", strlen(tttt), tttt);
    for (size_t ccc = 0; ccc < KEYNAME_SIZE; ccc++)
      printf(" %x", tttt[ccc]);
    printf("\n");
  }

void print_profile_info(profile_info *pinfo)
{
  if(pinfo == NULL)
    return;
  printf("--------\n");
  printf("is_loaded: %d\n", pinfo->is_loaded);
  printf("pf_name: %s\n", pinfo->pf_name);
  // printf("dim_unused_keys: %d\n", pinfo->dim_unused_keys);
  // for (size_t i = 0; i < TOTAL_OBSW_COUNT; i++)
  // {
  //   if(strlen(pinfo->sw_name[i]) == 0)
  //     continue;
  //   printf("key %d: %s\n", i, pinfo->sw_name[i]);
  //   printf("sw_color %d %d %d\n", pinfo->sw_color[i][0], pinfo->sw_color[i][1], pinfo->sw_color[i][2]);
  //   printf("sw_activation_color %d %d %d\n", pinfo->sw_activation_color[i][0], pinfo->sw_activation_color[i][1], pinfo->sw_activation_color[i][2]);
  //   printf(".\n");
  // }
  printf("--------\n");
}

if(is_sleeping && is_plus_minus_button(this_sw_event->id) && this_sw_event->type != SW_EVENT_RELEASE)
  {
    return;
  }
  else if(is_sleeping && this_sw_event->type == SW_EVENT_SHORT_PRESS)
  {
    wakeup_from_sleep_and_load_profile(current_profile_number);
    return;
  }

void update_ball_pos(void)
{
  ball_pos_x += ball_velocity_x;
  ball_pos_y += ball_velocity_y;

  if((ball_pos_x >= 127 - BALL_RADIUS*2) || (ball_pos_x <= BALL_RADIUS*2))
    ball_velocity_x = ball_velocity_x * -1;
  
  if((ball_pos_y >= 127 - BALL_RADIUS*2) || (ball_pos_y <= BALL_RADIUS*2))
    ball_velocity_y = ball_velocity_y * -1;
}

  while(1)
  {
    // uint8_t key_stat = readkey_nonblocking();
    // if(key_stat == 5)
    //   paddle0_pos += 5;
    // if(key_stat == 1)
    //   paddle0_pos -= 5;
    // printf("1: %ld %d\n", re_last_event_ts[0], re_last_event_dir[0]);
    // printf("2: %ld %ld\n", re_last_event_ts[1], re_last_event_dir[1]);
    sw_scan();
    for (size_t i = RE1_CW; i < SW_PLUS; i++)
      printf("%d ", poll_sw_state(i, 0));
    printf("\n");

    // update_ball_pos();
    // draw_court();
    // draw_ball(); 
    // ssd1306_UpdateScreen();
    delay_ms(66);
  }


    printf("mmov %d %d\n", this_x_amount*xsign, this_y_amount*ysign);
printf("!!! %x %x %x\n", combocode, ktype, kcode);

    printf("new data!\n");
    for (int i = 0; i < 16; ++i)
        printf("%x ", this_buf[i]);
    printf("\ndone\n");
uint8_t is_numlock_on, is_capslock_on, is_scrolllock_on;    
draw_kbled_icon(kb_led_status, 0);
void update_keyboard_led_status(uint8_t value)
{
  is_numlock_on = value & 0x1;
  is_capslock_on = value & 0x2;
  is_scrolllock_on = value & 0x4;
}
    printf("set %d %d %d %d\n", instance, report_id, report_type, bufsize);
    for (uint8_t i = 0; i < bufsize; i++)
      printf("%d ", buffer[i]);
    printf("\n");
  printf("%d %d %d\n", is_numlock_on, is_capslock_on, is_scrolllock_on);


#define HID_TX_BUF_SIZE HID_CC_IN_RPT_LEN-1
uint8_t test_buf[HID_TX_BUF_SIZE];
void my_kb_test(void)
{
    memset(test_buf, 0, HID_TX_BUF_SIZE);
    test_buf[2] = 0x21;
    ble_kb_send(test_buf);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    memset(test_buf, 0, HID_TX_BUF_SIZE);
    ble_kb_send(test_buf);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void my_mk_test(void)
{
    memset(test_buf, 0, HID_TX_BUF_SIZE);
    test_buf[0] = 64;
    int result = esp_hidd_dev_input_set(s_ble_hid_param.hid_dev, 0, 2, test_buf, 2);
    printf("MK result: %d\n", result);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    memset(test_buf, 0, HID_TX_BUF_SIZE);
    result = esp_hidd_dev_input_set(s_ble_hid_param.hid_dev, 0, 2, test_buf, 2);
    printf("MK result: %d\n", result);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void my_mouse_test(void)
{
    memset(test_buf, 0, HID_TX_BUF_SIZE);
    test_buf[1] = 16;
    int result = esp_hidd_dev_input_set(s_ble_hid_param.hid_dev, 0, 3, test_buf, 4);
    printf("MOUSE result: %d\n", result);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
uint32_t start = pdTICKS_TO_MS(xTaskGetTickCount());
printf("took %ldms\n", pdTICKS_TO_MS(xTaskGetTickCount()) - start);

uint8_t sw_queue_has_keydown_event(void)


  else if(this_opcode == OP_VMINFO)
  {
    exe->next_pc = (byte0 + 1) * INSTRUCTION_SIZE_BYTES;
  }

/*
ESP32 bare chip
4x5 20 keys in standard numpad shape
128x128 OLED screen
one or two EC11 rotary encoders
extra USB-C connectors for external accessories? like big red button or additional encoder
optional battery kit and battery management PCB?
sloped case design for battery kit?

new features:

more keys
rotary encoders
larger OLED
expansion module (custom buttons / foot pedal, etc)
bluetooth / wifi
much faster UI
more profiles
more sturdy +- button
easier to develop (esp-idf)

*/

for (uint8_t rrr = 0; rrr < SW_MATRIX_NUM_ROWS; rrr++)
  {
    for (int8_t ccc = SW_MATRIX_NUM_COLS; ccc >= 0; ccc--)
    {
      memset(line1_buf, 0, TEMP_BUFSIZE);
      memset(line2_buf, 0, TEMP_BUFSIZE);
      printf("x%dy%d ", ccc, rrr);
      // draw_cell_content(ccc, rrr, line1_buf, line2_buf);
    }
    printf("\n");
  }

  for (int8_t ccc = SW_MATRIX_NUM_COLS-1; ccc >= 0; ccc--)
  {
    for (int8_t rrr = 0; rrr < SW_MATRIX_NUM_ROWS; rrr++)
    {
      memset(line1_buf, 0, TEMP_BUFSIZE);
      memset(line2_buf, 0, TEMP_BUFSIZE);
      sprintf(line1_buf, "r%d", rrr);
      printf("x%dy%d ", ccc, rrr);
      draw_cell_content_rotated(ccc, rrr, line1_buf, line2_buf);
    }
    printf("\n");
  }


void ssd1306_dashed_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color) {
    int32_t deltaX = abs(x2 - x1);
    int32_t deltaY = abs(y2 - y1);
    int32_t signX = ((x1 < x2) ? 1 : -1);
    int32_t signY = ((y1 < y2) ? 1 : -1);
    int32_t error = deltaX - deltaY;
    int32_t error2;
    
    ssd1306_DrawPixel(x2, y2, color);
    uint8_t counter = 1;
    while((x1 != x2) || (y1 != y2)) {
        if(counter % 4 == 0)
            ssd1306_DrawPixel(x1, y1, color);
        error2 = error * 2;
        if(error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        
        if(error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
        counter++;
    }
    return;
}

printf("usb: ");
        for (size_t i = 0; i < 8; i++)
            printf("%x ", usb_hid_buf[i]);
        printf("\n");

        bt_hid_buf[0] = usb_hid_buf[1];
        bt_hid_buf[1] = usb_hid_buf[2];
        ble_mk_send(bt_hid_buf);

        printf("bt: ");
        for (size_t i = 0; i < 6; i++)
            printf("%x ", bt_hid_buf[i]);
        printf("\n");
print("---------")






  memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
  sprintf(oled_line_buf, "4-Use BT:Auto");
  ssd1306_SetCursor(0, 84);
  ssd1306_WriteString(oled_line_buf, Font_7x10, White);

uint32_t last_bt_draw = 0;
void draw_bluetooth_icon(uint8_t origx, uint8_t origy, uint8_t bt_stat)
{
  uint32_t now = pdTICKS_TO_MS(xTaskGetTickCount());
  if(now - last_bt_draw < 1000)
    return;
  if(bt_stat == BT_DISABLED)
    return;
  ssd1306_DrawPixel(origx+2, origy+0, White);
  ssd1306_DrawPixel(origx+2, origy+1, White);
  ssd1306_DrawPixel(origx+3, origy+1, White);
  ssd1306_DrawPixel(origx+2, origy+2, White);
  ssd1306_DrawPixel(origx+4, origy+2, White);
  ssd1306_DrawPixel(origx+0, origy+3, White);
  ssd1306_DrawPixel(origx+2, origy+3, White);
  ssd1306_DrawPixel(origx+5, origy+3, White);
  ssd1306_DrawPixel(origx+1, origy+4, White);
  ssd1306_DrawPixel(origx+2, origy+4, White);
  ssd1306_DrawPixel(origx+4, origy+4, White);
  ssd1306_DrawPixel(origx+2, origy+5, White);
  ssd1306_DrawPixel(origx+3, origy+5, White);
  ssd1306_DrawPixel(origx+1, origy+6, White);
  ssd1306_DrawPixel(origx+2, origy+6, White);
  ssd1306_DrawPixel(origx+4, origy+6, White);
  ssd1306_DrawPixel(origx+0, origy+7, White);
  ssd1306_DrawPixel(origx+2, origy+7, White);
  ssd1306_DrawPixel(origx+5, origy+7, White);
  ssd1306_DrawPixel(origx+2, origy+8, White);
  ssd1306_DrawPixel(origx+4, origy+8, White);
  ssd1306_DrawPixel(origx+2, origy+9, White);
  ssd1306_DrawPixel(origx+3, origy+9, White);
  ssd1306_DrawPixel(origx+2, origy+10, White);

  ssd1306_SetCursor(origx+7, origy+2);
  if(bt_stat == BT_DISCOVERABLE)
    ssd1306_WriteString("?", Font_6x8, White);
  else
    ssd1306_WriteString(" ", Font_6x8, White);

  ssd1306_UpdateScreen();
  last_bt_draw = now;
  // printf("bt draw!!!!\n");
}

void ble_kb_send(uint8_t* hid_buf, uint8_t bufsize)
{
    for (size_t i = 0; i < bufsize; i++)
        printf("%02d ", hid_buf[i]);
    printf("\n");
    int result = esp_hidd_dev_input_set(s_ble_hid_param.hid_dev, 0, HID_RPT_ID_CC_IN, hid_buf, HID_CC_IN_RPT_LEN);
    printf("send result: %d\n", result);
    result = esp_hidd_dev_battery_set(s_ble_hid_param.hid_dev, 50);
    printf("battery result: %d\n", result);
}

E (14659) BT_SMP: Value for numeric comparison = 793431
I (14661) ESP_HID_GAP: BLE GAP NC_REQ passkey:793431
draw_bluetooth_icon(0, -1, bluetooth_status);
    // while(1)
    // {
    //     vTaskDelay(pdMS_TO_TICKS(33));
    //     switch_event_t sw_event = { 0 };
    //     if(xQueueReceive(switch_event_queue, &sw_event, 0) == pdFALSE)
    //         continue;
    //     if(sw_event.type == SW_EVENT_SHORT_PRESS)
    //     {
    //         printf("!!!! key pressed\n");
    //         my_kb_test();
    //         my_mk_test();
    //         my_mouse_test();
    //     }
    // }
void fw_update_check(void)
{
  if(find_firmware_file(filename_buf, FILENAME_BUFSIZE))
  {
    printf("FIRMWARE FOUND: %s\n", filename_buf);
    uint32_t crc32 = calculate_crc32(filename_buf);
    printf("CRC32: %lx\n", crc32);
    
    sprintf(temp_buf, "%lx", crc32);
    if(strstr(filename_buf, temp_buf) == NULL)
    {
      // CRC mismatch, print error
      neopixel_fill(128, 0, 0);
      draw_fw_crc_error(filename_buf);
      return;
    }
    else
    {
      neopixel_fill(64, 64, 64);
      draw_fw_update_ask(filename_buf);
    }
  }
}

dpp_config.txt
"/sdcard/dp_stats.txt"
const char old_settings_file_path[] = "/sdcard/dp_settings.txt";
const char config_sleep_after_min[] = "sleep_after_min ";
const char config_brightness_index[] = "brightness_index ";
const char config_keyboard_layout[] = "kb_layout ";
const char settings_file_path[] = "/sdcard/dpp_config.txt";

  printf("swid: %d\n", swid);
printf("here! %s\n", dsb_on_press_path_buf);

  printf("reid: %d pos: %ld, dir: %d\n", this_re_event->state.id, this_re_event->state.position, this_re_event->state.direction);
  // sprintf(dsb_on_press_path_buf, "/sdcard/%s/key%d.dsb", all_profile_info[current_profile_number].dir_path, swid+1);

    printf("here! %s\n", dsb_on_press_path_buf);
void onboard_switch_press(uint8_t swid, char* press_path, char* release_path)
{
  if(strlen(all_profile_info[current_profile_number].sw_name_firstline[swid]) == 0)
    return;
  if(access(press_path, F_OK))
  {
    draw_red();
    draw_nodsb(swid);
    block_until_anykey();
    goto_profile(current_profile_number);
    return;
  }
  play_keydown_animation(current_profile_number, swid);
  key_press_count[swid]++;
  //-------------
  if(run_once(swid, press_path) == DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY)
    return;
  //--------------

  uint32_t hold_start = pdTICKS_TO_MS(xTaskGetTickCount());
  while(1)
  {
    if(poll_sw_state(swid) == 0)
      goto handle_obsw_keydown_end;
    if(pdTICKS_TO_MS(xTaskGetTickCount())- hold_start > 500)
      break;
  }
  while(1)
  {
    if(poll_sw_state(swid) == 0)
      break;
    key_press_count[swid]++;
    if(run_once(swid, press_path) == DSB_DONT_PLAY_KEYUP_ANIMATION_RETURN_IMMEDIATELY)
      return;
  }

  handle_obsw_keydown_end:
  // play keyup animation only if there is no on-release DSB file 
  if(access(release_path, F_OK))
    play_keyup_animation(current_profile_number, swid);
}


if(epilogue_value & EPILOGUE_SAVE_COLOR_STATE)
    {
      get_current_color(i, &red, &green, &blue);
    }
    else
    {
      red = all_profile_info[current_profile_number].sw_color[i][0];
      green = all_profile_info[current_profile_number].sw_color[i][1];
      blue = all_profile_info[current_profile_number].sw_color[i][2];
    }
void save_persistent_state(uint8_t epilogue_value)
{
  // use a different file name for duckypad pro!
  memset(sps_bin_buf, 0, SPS_BIN_SIZE);
  if(epilogue_value & EPILOGUE_SAVE_LOOP_STATE)
    memcpy(sps_bin_buf, key_press_count, MAX_TOTAL_SW_COUNT);
  for (uint8_t i = 0; i < NEOPIXEL_COUNT; i++)
  {
    uint8_t r_addr = i*3 + COLOR_START_ADDR;
    uint8_t g_addr = r_addr + 1;
    uint8_t b_addr = g_addr + 1;
    uint8_t red, green, blue;
    if(epilogue_value & EPILOGUE_SAVE_COLOR_STATE)
    {
      get_current_color(i, &red, &green, &blue);
    }
    else
    {
      red = all_profile_info[current_profile_number].sw_color[i][0];
      green = all_profile_info[current_profile_number].sw_color[i][1];
      blue = all_profile_info[current_profile_number].sw_color[i][2];
    }
    sps_bin_buf[r_addr] = red;
    sps_bin_buf[g_addr] = green;
    sps_bin_buf[b_addr] = blue;
  }
  
  memset(temp_buf, 0, TEMP_BUFSIZE);
  sprintf(temp_buf, "/sdcard/%s/state.sps", all_profile_info[current_profile_number].dir_path);
  // printf("%s\n", temp_buf);
  remove(temp_buf);

  memset(temp_buf, 0, TEMP_BUFSIZE);
  sprintf(temp_buf, "/sdcard/%s/state_dpp.sps", all_profile_info[current_profile_number].dir_path);
  // printf("%s\n", temp_buf);

  FILE *file = fopen(temp_buf, "wb");
  if (file == NULL)
    return;
  fwrite(sps_bin_buf, 1, SPS_BIN_SIZE, file);
  fclose(file);
  // for (uint8_t i = 0; i < MAX_TOTAL_SW_COUNT; i++)
  //   printf("%d ", key_press_count[i]);
  // printf("\ndone???\n");
}

      printf("%s %s\n", dsb_on_press_path_buf, dsb_on_release_path_buf);
printf("what_to_do: %d\n", what_to_do);
#define PATH_BUF_SIZE 128
char dsb_path_buf[PATH_BUF_SIZE];
char dsb_on_release_path_buf[PATH_BUF_SIZE];


char dsb_on_release_path_buf[PATH_BUF_SIZE];

memset(dsb_path_buf, 0, PATH_BUF_SIZE);
  sprintf(dsb_path_buf, "/sdcard/%s/key%d.dsb", all_profile_info[current_profile_number].dir_path, swid+1);

memset(dsb_on_release_path_buf, 0, PATH_BUF_SIZE);
sprintf(dsb_on_release_path_buf, "/sdcard/%s/key%d-release.dsb", all_profile_info[current_profile_number].dir_path, swid+1);





MY_UNIMPLEMENTED

void redraw_bg(uint8_t profile_number)
{
  if(profile_number >= MAX_PROFILES)
    return;
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
  {
    neo_anime[i].animation_type = ANIMATION_NONE;
    set_pixel_color(i, all_profile_info[profile_number].sw_color[i]);
    if(all_profile_info[profile_number].dim_unused_keys)
  }
  neopixel_draw_current_buffer();
}

void redraw_bg(uint8_t profile_number)
{
  for (int i = 0; i < NEOPIXEL_COUNT; ++i)
  {
    neo_anime[i].animation_type = ANIMATION_NONE;
    set_pixel_color(i, all_profile_info[profile_number].sw_color[i]);
  }
  neopixel_draw_current_buffer();
}


  // printf("held more than 500ms!!!\n");
  while(1)
  {
    if(poll_sw_state(swid) == 0)
      break;
    // printf("still holding!!!\n");
    run_once(swid, temp_buf);
    // vTaskDelay(pdMS_TO_TICKS(INPUT_TASK_FREQ_MS*2));
  }

  while(poll_sw_state(swid))
  {
    printf("still pressed!!!\n");
    vTaskDelay(pdMS_TO_TICKS(100));
  }

FF_USE_LABEL

for (size_t i = 0; i < HID_TX_BUF_SIZE; i++)
        printf("%02d ", buffer[i]);
    printf("\n");

printf("should_mount_usb_msc: %d\n", );
    memset(temp_buf, 0, TEMP_BUFSIZE);
    f_getlabel("", temp_buf, 0);
    printf("vol label is: %s\n", temp_buf);
    printf("new data!\n");
    for (int i = 0; i < rx_buf_size; ++i)
        printf("0x%x ", hid_rx_buf[i]);
    printf("\ndone\n");

    char ttttt[12];
    f_getlabel("", ttttt, 0);
    printf("vol label is: %s\n", ttttt);

void whoops(uint8_t* hid_cmdbuf)
{
    printf("whoops\n");
    for (int i = 0; i < HID_TX_BUF_SIZE; ++i)
        printf("0x%x ", hid_cmdbuf[i]);
    printf("\n--------\n");
    tud_hid_report(HID_USAGE_ID_NAMED_PIPE, hid_cmdbuf, HID_TX_BUF_SIZE-1);
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    // printf("\n\ngot data!\n");
    // printf("%d %d %d %d\n", instance, report_id, report_type, bufsize);
    // for (size_t i = 0; i < bufsize; i++)
    //     printf("%d ", buffer[i]);
    // printf("\n\n");

    handle_hid_command(buffer, bufsize);
}

 /*
    HID LIST FILES
    -----------
    PC to duckyPad:
    [0]   seq number
    [1]   command
    [2 ... 63]   starting directory, zero-terminated string, all 0 for root
    -----------
    duckyPad to PC
    [0]   seq number (not used)
    [1]   0 = OK, 1 = ERROR, 2 = BUSY, 3 = EOF
    [2]   file type, 0 = file, 1 = directory
    [3 ... 63] zero-terminated string of file name
    */
    else if(command_type == HID_COMMAND_LIST_FILES)
    {
        char* dir_path = "/sdcard";
        if(hid_rx_buf[2])
            dir_path = hid_rx_buf[2];
        
        struct dirent *dir;
        DIR *d = opendir(dir_path);
        if(d == NULL)
            goto list_file_end;
        while(1)
        {
            hid_rx_has_unprocessed_data = 0;
            memset(lfn_buf, 0, FILENAME_SIZE);
            memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
            hid_tx_buf[0] = 4;
            hid_tx_buf[1] = 0;
            hid_tx_buf[2] = HID_RESPONSE_OK;
            
            if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0)
                break;
            if (fno.fattrib & AM_DIR)
                hid_tx_buf[3] = 1;
            this_filename = fno.lfname[0] ? fno.lfname : fno.fname;
            // if(strstr(this_filename, ".dsb")) // saves some time skipping dsb file
            //   continue;
            strncpy(hid_tx_buf+4, this_filename, FILENAME_SIZE);
            send_hid_cmd_response(hid_tx_buf);
            
            if(check_resume() == 0)
                goto hid_read_file_end;

        }
        list_file_end:
        memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
        hid_tx_buf[0] = 0; 
        hid_tx_buf[1] = HID_RESPONSE_EOF;
        send_hid_cmd_response(hid_tx_buf);
        closedir(d);
        hid_rx_has_unprocessed_data = 0;
    }
        

void USBD_CUSTOM_HID_SendReport(uint8_t* hid_buf)
{
    uint8_t hid_usage_type = hid_buf[0];
    memset(esp_hid_msg, 0, EIGHT);
    if(hid_usage_type == HID_USAGE_ID_KEYBOARD)
    {
        memcpy(esp_hid_msg, hid_buf, SIX);
        esp_hid_msg[0] = hid_buf[1]; // modifier
        esp_hid_msg[1] = 0; // reserved
    }
    else if(hid_usage_type == HID_USAGE_ID_MOUSE)
    {
        memcpy(esp_hid_msg, hid_buf+1, SIX-1);
    }
    else if(hid_usage_type == HID_USAGE_ID_MEDIA_KEY)
    {
        esp_hid_msg[0] = hid_buf[1];
    }
    else if(hid_usage_type == HID_USAGE_ID_NAMED_PIPE)
    {
        memcpy(esp_hid_msg, hid_buf, SIX);
    }
    tud_hid_report(hid_usage_type, esp_hid_msg, sizeof(esp_hid_msg));
}


// ----------------- HID command parsing -------------------

uint8_t command_type;
uint8_t hid_tx_buf[HID_TX_BUF_SIZE];

void handle_hid_command(char* hid_rx_buf, uint8_t rx_buf_size)
{
  printf("new data!\n");
  for (int i = 0; i < rx_buf_size; ++i)
    printf("%c ", hid_rx_buf[i]);
  printf("\ndone\n");

  command_type = hid_rx_buf[2];

  memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
  hid_tx_buf[0] = 4;
  hid_tx_buf[1] = 0;
  hid_tx_buf[2] = HID_RESPONSE_OK;

  /*
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR, 2 = BUSY
  */
  if(is_busy)
  {
    hid_tx_buf[2] = HID_RESPONSE_BUSY;
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    return;
  }

  /*
  HID GET INFO
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK
  [3]   firmware version major
  [4]   firmware version minor
  [5]   firmware version patch
  [6]   hardware revision
  [7 - 10]   UUID (uint32_t)
  [11]   current profile
  [12] is_sleeping
  */
  if(command_type == HID_COMMAND_GET_INFO)
  {
    hid_tx_buf[3] = fw_version_major;
    hid_tx_buf[4] = fw_version_minor;
    hid_tx_buf[5] = fw_version_patch;
    hid_tx_buf[6] = 24;
    hid_tx_buf[7] = esp_mac_addr[2];
    hid_tx_buf[8] = esp_mac_addr[3];
    hid_tx_buf[9] = esp_mac_addr[4];
    hid_tx_buf[10] = esp_mac_addr[5];
    hid_tx_buf[11] = current_profile_number;
    hid_tx_buf[12] = is_sleeping;
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
  /*
  HID GOTO PROFILE
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  [3]   profile number to switch to
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR
  */
  else if(command_type == HID_COMMAND_GOTO_PROFILE)
  {
    if(p_cache.available_profile[hid_rx_buf[3]])
    {
      USBD_CUSTOM_HID_SendReport(hid_tx_buf);
      is_sleeping = 0;
      oled_full_brightness();
      restore_profile(hid_rx_buf[3]);
    }
    else
    {
      hid_tx_buf[2] = HID_RESPONSE_ERROR;
      USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    }
  }
  /*
  HID PREV PROFILE
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK
  */
  else if(command_type == HID_COMMAND_PREV_PROFILE)
  {
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    oled_full_brightness();
    change_profile(PREV_PROFILE);
    is_sleeping = 0;
  }
  /*
  HID NEXT PROFILE
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK
  */
  else if(command_type == HID_COMMAND_NEXT_PROFILE)
  {
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    oled_full_brightness();
    change_profile(NEXT_PROFILE);
    is_sleeping = 0;
  }

  /*
  HID LIST FILES
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  [3 ... 63]   starting directory, zero-terminated string, all 0 for root
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR, 2 = BUSY, 3 = EOF
  [3]   file type, 0 = file, 1 = directory
  [4 ... 63] zero-terminated string of file name
  */
  else if(command_type == HID_COMMAND_LIST_FILES)
  {
    char* this_filename;
    fno.lfname = lfn_buf; 
    fno.lfsize = FILENAME_SIZE - 1;
    if (f_opendir(&dir, hid_rx_buf+3) != FR_OK)
      goto list_file_end;
    memset(temp_buf, 0, PATH_SIZE);
    while(1)
    {
      hid_rx_has_unprocessed_data = 0;
      memset(lfn_buf, 0, FILENAME_SIZE);
      memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
      hid_tx_buf[0] = 4;
      hid_tx_buf[1] = 0;
      hid_tx_buf[2] = HID_RESPONSE_OK;
      
      if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0)
        break;
      if (fno.fattrib & AM_DIR)
        hid_tx_buf[3] = 1;
      this_filename = fno.lfname[0] ? fno.lfname : fno.fname;
      // if(strstr(this_filename, ".dsb")) // saves some time skipping dsb file
      //   continue;
      strncpy(hid_tx_buf+4, this_filename, FILENAME_SIZE);
      USBD_CUSTOM_HID_SendReport(hid_tx_buf);
      
      if(check_resume() == 0)
        goto hid_read_file_end;

    }
    list_file_end:
    memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
    hid_tx_buf[0] = 4;
    hid_tx_buf[1] = 0;
    hid_tx_buf[2] = HID_RESPONSE_EOF;
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    f_closedir(&dir);
    hid_rx_has_unprocessed_data = 0;
  }
  /*
  HID READ FILE
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  [3 ... 63]   file path, zero-terminated string
  -----------
  duckyPad to PC
  [1]   seq number, xxxincrementing
  [2]   0 = OK, 1 = ERROR, 2 = BUSY, 3 = EOF
  [3 ... 60] file content
  */
  else if(command_type == HID_COMMAND_READ_FILE)
  {
    if(f_open(&sd_file, hid_rx_buf+3, FA_READ) != 0)
      goto hid_read_file_end;

    while(1)
    {
      hid_rx_has_unprocessed_data = 0;
      memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
      memset(read_buffer, 0, READ_BUF_SIZE);
      hid_tx_buf[0] = 4;
      hid_tx_buf[2] = HID_RESPONSE_OK;
      f_read(&sd_file, read_buffer, HID_FILE_READ_BUF_SIZE, &bytes_read);
      memcpy(hid_tx_buf+3, read_buffer, bytes_read);
      hid_tx_buf[1] = bytes_read;
      USBD_CUSTOM_HID_SendReport(hid_tx_buf);

      if(check_resume() == 0)
        goto hid_read_file_end;
      
      if(bytes_read < HID_FILE_READ_BUF_SIZE)
        break;
    }
    hid_read_file_end:
    f_close(&sd_file);
    memset(hid_tx_buf, 0, HID_TX_BUF_SIZE);
    hid_tx_buf[0] = 4;
    hid_tx_buf[1] = 0;
    hid_tx_buf[2] = HID_RESPONSE_EOF;
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    hid_rx_has_unprocessed_data = 0;
  }
  /*
  HID OPEN FILE FOR WRITING
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  [3 ... 63]   file path, zero-terminated string
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR, 2 = BUSY
  [3 ... 60] file content
  */
  else if(command_type == HID_COMMAND_OPEN_FILE_FOR_WRITING)
  {
    if(f_open(&sd_file, hid_rx_buf+3, FA_CREATE_ALWAYS | FA_WRITE) != 0)
    {
      hid_tx_buf[2] = HID_RESPONSE_ERROR;
      goto hid_open_for_write_end;
    }

    hid_tx_buf[2] = HID_RESPONSE_OK;
    hid_open_for_write_end:
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
  /*
  HID WRITE TO FILE
  -----------
  PC to duckyPad:
  [1]   data length in bytes
  [2]   command
  [3 ... 63]   content
  -----------
  duckyPad to PC
  [1]   reserved
  [2]   0 = OK, 1 = ERROR, 2 = BUSY
  */
  else if(command_type == HID_COMMAND_WRITE_FILE)
  {
    if(f_write(&sd_file, hid_rx_buf+3, hid_rx_buf[1], &bytes_read) != 0)
      hid_tx_buf[2] = HID_RESPONSE_ERROR;
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
  /*
  HID CLOSE FILE
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR, 2 = BUSY
  */
  else if(command_type == HID_COMMAND_CLOSE_FILE)
  {
    f_close(&sd_file);
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
  /*
  HID DELETE FILE
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  [3 ... 63]   file name string, zero terminated
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR, 2 = BUSY
  */
  else if(command_type == HID_COMMAND_DELETE_FILE)
  {
    f_close(&sd_file);
    // if(f_unlink(hid_rx_buf+3) != 0)
    //   hid_tx_buf[2] = HID_RESPONSE_ERROR;
    f_unlink(hid_rx_buf+3);
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
  /*
  HID create DIR
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  [3 ... 63]   directory name string, zero terminated
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR, 2 = BUSY
  */
  else if(command_type == HID_COMMAND_CREATE_DIR)
  {
    if(f_mkdir(hid_rx_buf+3) != 0)
      hid_tx_buf[2] = HID_RESPONSE_ERROR;
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
  /*
  HID DELETE DIR
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  [3 ... 63]   dir name string, zero terminated
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK, 1 = ERROR, 2 = BUSY
  */
  else if(command_type == HID_COMMAND_DELETE_DIR)
  {
    // if(delete_node(hid_rx_buf+3, rx_buf_size - 3, &fno) != 0)
    //   hid_tx_buf[2] = HID_RESPONSE_ERROR;
    delete_node(hid_rx_buf+3, rx_buf_size - 3, &fno);
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
  /*
  HID SOFTWARE RESET
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK
  */
  else if(command_type == HID_COMMAND_SW_RESET)
  {
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    osDelay(50);
    NVIC_SystemReset();
  }
  /*
  HID SLEEP
  -----------
  PC to duckyPad:
  [1]   seq number
  [2]   command
  -----------
  duckyPad to PC
  [1]   seq number (not used)
  [2]   0 = OK
  */
  else if(command_type == HID_COMMAND_SLEEP)
  {
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
    start_sleeping();
  }
  /*
    unknown command
    -----------
    duckyPad to PC
    [1]   seq number
    [2]   1 = ERROR
    */
  else
  {
    hid_tx_buf[2] = HID_RESPONSE_ERROR;
    USBD_CUSTOM_HID_SendReport(hid_tx_buf);
  }
}


esp_mac_addr[3], esp_mac_addr[4], esp_mac_addr[5]


void send_keyboard_hid_msg(uint8_t* hid_buf)
{
    // usage_id, modifier, rest of the buffer size 6
    tud_hid_keyboard_report(HID_USAGE_ID_KEYBOARD, hid_buf[1], hid_buf+2);
    
}

void app_send_hid_demo(void)
{
    /*
    tud_hid_report, first argument is usage ID, see HID descriptor, keyboard is 1, media
    key is 2, mouse is 3, etc
    can also use tud_hid_keyboard_report, but probably easier to use tud_hid_report
    */
    // Keyboard output: Send key 'a/A' pressed and released
    printf("Sending Keyboard report\n");
    uint8_t keycode[6] = {HID_KEY_A, 0, 0, 0, 0, 0};
    tud_hid_keyboard_report(1, 0, keycode);
    vTaskDelay(pdMS_TO_TICKS(100));
    tud_hid_keyboard_report(1, 0, NULL);
}

void USBD_CUSTOM_HID_SendReport(uint8_t* hid_buf)
{
    printf("HID: ");
    for (size_t i = 0; i < KB_BUF_SIZE; i++)
        printf("%02x ", hid_buf[i]);
    printf("\n");

    printf("tud: %d\n", );
}


void app_send_hid_demo(void)
{
    /*
    tud_hid_report, first argument is usage ID, see HID descriptor, keyboard is 1, media
    key is 2, mouse is 3, etc

    can also use tud_hid_keyboard_report, but probably easier to use tud_hid_report
    */
    // Keyboard output: Send key 'a/A' pressed and released
    printf("Sending Keyboard report\n");
    uint8_t keycode[6] = {0x40, 0, 0, 0, 0, 0};
    tud_hid_report(2, keycode, 6);
    vTaskDelay(pdMS_TO_TICKS(100));
    memset(keycode, 0, 6);
    tud_hid_report(2, keycode, 6);
}

  der_init(&this_exe);
  run_dsb(&this_exe, swid, temp_buf);

  app_send_hid_demo();
  memset(temp_buf, 0, TEMP_BUFSIZE);
  sprintf(temp_buf, "hello world");
  kb_print(temp_buf, 20, 0);


        printf("current keymap: %s\n", dp_settings.current_kb_layout);


  memset(dp_settings.current_kb_layout, 0, FILENAME_BUFSIZE);
  get_first_keymap(dp_settings.current_kb_layout);
  printf("first keymap: %s\n", dp_settings.current_kb_layout);


uint8_t get_next_keymap(char* current_keymap_filename, char* next_keymap_filename, uint8_t next_keymap_buf_size)
{
  struct dirent *dir;
  DIR *d = opendir("/sdcard/keymaps");
  printf("current keymap: %s\n", current_keymap_filename);
  if(d == NULL)
    return 1;
  uint8_t found = 0;
  while ((dir = readdir(d)) != NULL)
  {
    if(dir->d_type != DT_REG)
      continue;
    if(!(strncmp(dir->d_name, "dpkm_", 5) == 0 && strstr(dir->d_name, ".txt") != NULL))
      continue;
    if(found)
    {
      strcpy(next_keymap_filename, dir->d_name);
      closedir(d);
      return 0;
    }
    if(strcmp(dir->d_name, current_keymap_filename) == 0)
      found = 1;
    printf("gnk %s\n", dir->d_name);
  }
  closedir(d);
  return 2;
}
uint8_t load_dsb(char* dsb_path)
{
  FILE *sd_file = fopen(dsb_path, "r");
  if(sd_file == NULL)
    return DSB_FOPEN_FAIL;
  memset(bin_buf, 0, BIN_BUF_SIZE);
  int32_t result = fread(bin_buf, 1, BIN_BUF_SIZE, sd_file);
  printf("fread: %ld\n", result);
  return DSB_OK;
}
void get_rc(void)
{
  rotary_encoder_event_t event = { 0 };
  if (xQueueReceive(rotary_encoder_event_queue, &event, 0) == pdTRUE)
    printf("Event: id: %d pos: %ld, dir: %d\n", event.state.id, event.state.position, event.state.direction);
}

FILE *sd_file = fopen(temp_buf, "r");
  if(sd_file == NULL)
    return 2;

sprintf(dp_settings.current_kb_layout, "dpkm_Finnish.txt");
int fff = load_keymap_by_name(dp_settings.current_kb_layout);
printf("load_keymap_by_name result: %d\n", fff);


printf("duckcode_circumflex: 0x%04X\n", duckcode_circumflex);
printf("duckcode_diaeresis: 0x%04X\n", duckcode_diaeresis);
printf("duckcode_grave_accent: 0x%04X\n", duckcode_grave_accent);
printf("duckcode_acute_accent: 0x%04X\n", duckcode_acute_accent);
printf("duckcode_tilde: 0x%04X\n", duckcode_tilde);
printf("duckcode_cedilla: 0x%04X\n", duckcode_cedilla);
for (size_t i = 0; i < ASCII_MAP_SIZE; i++)
printf("0x%02x 0x%04x\n", i, ascii_map[i]);

extern uint16_t duckcode_circumflex;
extern uint16_t duckcode_diaeresis;
extern uint16_t duckcode_grave_accent;
extern uint16_t duckcode_acute_accent;
extern uint16_t duckcode_tilde;
extern uint16_t duckcode_cedilla;

void validate_keymap_name(void)
{
  if(strlen(dp_settings.current_kb_layout) == 0)
    strcpy(dp_settings.current_kb_layout, default_keymap_name);

  memset(temp_buf, 0, TEMP_BUFSIZE);
  sprintf(temp_buf, "/sdcard/keymaps/%s", dp_settings.current_kb_layout);
  printf("vali: %s\n", temp_buf);
}
printf("%s\n", temp_buf);
    memset(temp_buf, 0, TEMP_BUFSIZE);

if(xSemaphoreTake(neopixel_mutex, pdMS_TO_TICKS(NEOPIXEL_MUTEX_TIMEOUT_MS)) == pdFALSE)
    return;
  neopixel_show(red_buf, green_buf, blue_buf, brightness_index_to_percent_lookup[dp_settings.brightness_index]);
  xSemaphoreGive(neopixel_mutex);

OLED SC: 80
id: 20 type: 1
OLED SC: 255
id: 20 type: 0
OLED SC: 80
id: 20 type: 1
E (30936) spi_master: spi_device_acquire_bus(976): Cannot acquire bus when a polling transaction is in progress.
E (30936) spi_master: spi_device_polling_start(1042): Cannot send polling transaction while the previous polling transaction is not terminated.
E (30948) spi_master: Cannot release bus when a polling transaction is in progress.

assert failed: spi_device_release_bus spi_master.c:1012 (0)


Backtrace: 0x403758f6:0x3fcacc40 0x4037b859:0x3fcacc60 0x40381d11:0x3fcacc80 0x403789b7:0x3fcacda0 0x4200ac07:0x3fcacdc0 0x4200aee2:0x3fcacdf0 0x42009917:0x3fcace10 0x4037df11:0x3fcace40





ssd1306_SetContrast(255);


  if(swid >= MSW_0 && swid <= MAX_MSW)
  {
    if(event_type == SW_EVENT_SHORT_PRESS)
    {
      handle_keydown(swid);
    }
    else if(event_type == SW_EVENT_RELEASE)
    {
      handle_keyup(swid);
    }
  }

brightness_values

uint8_t save_settings(dp_global_settings* dps)
{
  memset(temp_buf, 0, TEMP_BUFSIZE);
  sprintf(temp_buf, "sleep_after_min %ld\nbi %d\nkbl %s\n", dps->sleep_after_ms/60000, dps->brightness_index, dps->current_kb_layout);
  printf("%s", temp_buf);
  return 0;
}

  void save_settings(void)
{
  if(f_open(&sd_file, "dp_settings.txt", FA_CREATE_ALWAYS | FA_WRITE) != 0)
    goto ss_end;
  memset(temp_buf, 0, PATH_SIZE);
  sprintf(temp_buf, "sleep_after_min %d\nbi %d\nkbl %s\n", dp_settings.sleep_after_ms/60000, brightness_index, curr_kb_layout);
  f_write(&sd_file, temp_buf, strlen(temp_buf), &bytes_read);
  ss_end:
  f_close(&sd_file);
}


  printf("goto_profile: %d\n", profile_number);
printf("lsa %d %d %f %d %f\n", i, dest_color[i], anime_struct->current_color[i], durations_frames, anime_struct->step[i]);
    printf("000 %d %ld\n", idx, current_frame);
    printf("fc %ld\n", frame_counter);
led_start_animation(&neo_anime[sw_number], color_red, ANIMATION_CROSS_FADE, 70);
void print_nomemory(void)
{
    ssd1306_Fill(Black);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Memory Allocation");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 35);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Failed!");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 47);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "Instructions:");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 80);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    memset(oled_line_buf, 0, OLED_LINE_BUF_SIZE);
    sprintf(oled_line_buf, "duckyPad.com");
    ssd1306_SetCursor(center_line(strlen(oled_line_buf), 7, SSD1306_WIDTH), 92);
    ssd1306_WriteString(oled_line_buf, Font_7x10, White);

    ssd1306_UpdateScreen();
}

  valid_profile_count = get_valid_profile_count();
  if(valid_profile_count == 0)
    return PSCAN_ERROR_NO_PROFILE;

void goto_next_profile(void)
{
  int16_t next_profile_number = current_profile_number;
  while(1)
  {
    next_profile_number++;
    if(next_profile_number >= MAX_PROFILES)
      next_profile_number = 0;
    if(profile_number_to_index_lookup[next_profile_number] != -1)
      break;
  }
  printf("now: %d\n", next_profile_number);
  int16_t next_profile_index = profile_number_to_index_lookup[next_profile_number];
  restore_profile(&all_profile_info[next_profile_index]);
  current_profile_number = next_profile_number;
}

  printf("get_first_valid_profile_number: %d\n", get_first_valid_profile_number());
void goto_next_profile(void)
{
  int8_t next_profile_index = 0;
  while(1)
  {
    printf("gnp: %d\n", current_profile_number);
    current_profile_number++;
    if(current_profile_number >= MAX_PROFILES)
      current_profile_number = 0;
    if(profile_number_to_index_lookup[current_profile_number] == -1)
      continue;
    break;
  }
  printf("now: %d\n", current_profile_number);
}

for (size_t i = 0; i < MECH_OBSW_COUNT; i++)
  {
    uint8_t row = 0;
    uint8_t col = 0;
    index_to_rowcol(i, &row, &col);
    printf("%d: row%d col%d\n", i, row, col);
  }
void index_to_rowcol(uint8_t index, uint8_t *row, uint8_t *col)
{
    if(index >= MECH_OBSW_COUNT)
        return;
    *row = index / SW_MATRIX_NUM_COLS;
    *col = index % SW_MATRIX_NUM_COLS;
}
draw_profile(&all_profile_info[1]);

printf("load_profile_config: %s\n", filename_buf);
  printf("parse_profile_config_line: %s\n", this_line);
void mytest(void)
{
  uint8_t last_used_profile = get_last_used_profile();
}
void fill_default_color(profile_info* this_profile)
{
  if(this_profile == NULL)
    return;
  for (size_t i = 0; i < TOTAL_OBSW_COUNT; i++)
  {
    this_profile->sw_color[i][0] = DEFAULT_BG_RED;
    this_profile->sw_color[i][1] = DEFAULT_BG_GREEN;
    this_profile->sw_color[i][2] = DEFAULT_BG_BLUE;

    this_profile->sw_activation_color[i][0] = DEFAULT_KD_RED;
    this_profile->sw_activation_color[i][1] = DEFAULT_KD_GREEN;
    this_profile->sw_activation_color[i][2] = DEFAULT_KD_BLUE;
  }
}

    printf("this_key_index %d\n", this_key_index);
void parse_key_config_line(char* line, uint32_t buf_size, profile_info* this_profile)
{
  if(line == NULL || strlen(line) <= 2)
    return;
  printf("parse_key_config_line: %s\n", line);

  if(line[0] == 'z')
  {
    uint8_t this_key_index = atoi(line+1);
    if(this_key_index == 0)
      return;
    this_key_index--;
    if(this_key_index >= MAPPABLE_OBSW_COUNT)
      return;
    printf("111 %d\n", this_key_index);
    memset(this_profile->sw_name[this_key_index], 0, KEYNAME_SIZE);
    printf("222 %d\n", this_key_index);
    char* kn_start = goto_next_arg(line, buf_size);
    printf("zz: %d %s\n", this_key_index, kn_start);
  }

}
uint8_t get_last_used_profile(void)
{
  uint8_t ret = 0;

  FILE *sd_file = fopen("/sdcard/dp_stats.txt", "r");
  if(sd_file == NULL)
    return 1;

  memset(temp_buf, 0, TEMP_BUFSIZE);
  while(fgets(temp_buf, TEMP_BUFSIZE, sd_file))
  {
    if(strncmp(temp_buf, stat_last_used_profile, strlen(stat_last_used_profile)) == 0)
      ret = atoi(temp_buf+strlen(stat_last_used_profile));
  }

  if(ret >= MAX_PROFILES)
    ret = 0;

  fclose(sd_file);
  return ret;
}

      printf("%d %d: %s\n", count, this_profile_number, dir->d_name);
void load_keynames(uint8_t pf_size)
{
  for (uint8_t i = 0; i < pf_size; i++)
  {
    // print_profile_info(&all_profile_info[i]);
    memset(filename_buf, 0, FILENAME_BUFSIZE);
    sprintf(filename_buf, "%s/%s", SD_MOUNT_POINT, all_profile_info[i].dir_path);
    printf("%s\n", filename_buf);
  }
}

profile_info current_profile_info;
printf("jjj %d\n", valid_profile_count);
void load_profile(uint8_t pid)
{
  ;
}

#define FILENAME_BUFSIZE 64

typedef struct
{
    uint8_t profile_id;
    char name[FILENAME_BUFSIZE];
} profile_info;

uint8_t get_profile_dirname(uint8_t pid, char* pname_buf, uint32_t pname_buf_size)
{
  struct dirent *dir;
  DIR *d = opendir(SD_MOUNT_POINT);
  memset(pname_buf, 0, pname_buf_size);
  if(d) 
  {
    while ((dir = readdir(d)) != NULL)
    {
      if(dir->d_type != DT_DIR)
        continue;
      if(is_profile_dir(dir->d_name) == 0)
        continue;
      if(atoi(dir->d_name + strlen(profile_dir_prefix)) != pid)
        continue;
      available_profile_list[this_profile_number] = 1;
    }
  }
  closedir(d);
}


void scan_profiles()
{
  struct dirent *dir;
  DIR *d = opendir(SD_MOUNT_POINT);
  memset(available_profile_list, 0, MAX_PROFILES);
  if(d) 
  {
    while ((dir = readdir(d)) != NULL)
    {
      if(dir->d_type != DT_DIR)
        continue;
      if(is_profile_dir(dir->d_name) == 0)
        continue;
      uint8_t this_profile_number = atoi(dir->d_name + strlen(profile_dir_prefix));
      if(this_profile_number >= MAX_PROFILES)
        continue;
      available_profile_list[this_profile_number] = 1;
    }
  }
  closedir(d);
}


void mytest(void)
{
  uint8_t last_used_profile = get_last_used_profile();
  if(last_used_profile == 0)
    change_profile(NEXT_PROFILE);
  else
    restore_profile(last_profile);
}
void mytest(void)
{
  // printf("mytest: %d\n", get_last_used_profile());
  scan_profiles();
  for (int i = 0; i < MAX_PROFILES; i++)
  {
    if(available_profile_list[i])
      printf("%d\n", i);
  }
  
}


void step_profile(uint8_t direction)
{
  
}


    printf("%d %d %ld %s\n", sdf, dp_settings.brightness_index, dp_settings.sleep_after_ms, dp_settings.current_kb_layout);


--------------

#include <dirent.h> 

int8_t list_files(void)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(SD_MOUNT_POINT);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return 0;
}

void get_rc(void)
{
    UBaseType_t items_in_queue = uxQueueMessagesWaiting(rotary_encoder_event_queue);
    printf("queue: %d\n", items_in_queue);
    // Wait for incoming events on the event queue.
    rotary_encoder_event_t event = { 0 };
    if (xQueueReceive(rotary_encoder_event_queue, &event, 0) == pdTRUE)
        printf("Event: id: %d pos: %ld, dir: %d\n", event.state.id, event.state.position, event.state.direction);
}


-------------


void input_test(void)
{
    memset(this_sw_matrix_state, 0, SW_MATRIX_BUF_SIZE);
    gpio_set_level(SWM_COL0_GPIO, 1);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 0);
    scan_row(0);

    gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 1);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 0);
    scan_row(1);

    gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 1);
    gpio_set_level(SWM_COL3_GPIO, 0);
    scan_row(2);

    gpio_set_level(SWM_COL0_GPIO, 0);
    gpio_set_level(SWM_COL1_GPIO, 0);
    gpio_set_level(SWM_COL2_GPIO, 0);
    gpio_set_level(SWM_COL3_GPIO, 1);
    scan_row(3);

    for (int rrr = 0; rrr < SW_MATRIX_NUM_ROWS; ++rrr)
    {
        for (int ccc = 0; ccc < SW_MATRIX_NUM_COLS; ++ccc)
        {
            printf("R%dC%dS%d ", rrr, ccc, this_sw_matrix_state[rowcol_to_index(rrr, ccc)]);
            // printf("%d ", this_sw_matrix_state[rowcol_to_index(rrr, ccc)]);
        }
        printf("\n");
    }

    printf("---------------\n");
}

R0C0S1 R1C0S0 R2C0S0 R3C0S0 R4C0S0 
R0C1S0 R1C1S0 R2C1S0 R3C1S0 R4C1S0 
R0C2S0 R1C2S0 R2C2S0 R3C2S0 R4C2S0 
R0C3S1 R1C3S0 R2C3S0 R3C3S0 R4C3S0 

--------------------------

void input_test(void)
{
    // gpio_set_level(SWM_COL0_GPIO, 1);
    // gpio_set_level(SWM_COL1_GPIO, 0);
    // gpio_set_level(SWM_COL2_GPIO, 0);
    // gpio_set_level(SWM_COL3_GPIO, 0);

    // printf("%d ", gpio_get_level(SWM_ROW0_GPIO));
    // printf("%d ", gpio_get_level(SWM_ROW1_GPIO));
    // printf("%d ", gpio_get_level(SWM_ROW2_GPIO));
    // printf("%d ", gpio_get_level(SWM_ROW3_GPIO));
    // printf("%d\n", gpio_get_level(SWM_ROW4_GPIO));

    for (int ccc = 0; ccc < SW_MATRIX_NUM_COLS; ++ccc)
    {
        for (int rrr = 0; rrr < SW_MATRIX_NUM_ROWS; ++rrr)
        {
            printf("R%dC%dI%02d ", rrr, ccc, rowcol_to_index(rrr, ccc));
        }
        printf("\n");
    }

    printf("---------------\n");
}



R0C0I00 R1C0I04 R2C0I08 R3C0I12 R4C0I16 
R0C1I01 R1C1I05 R2C1I09 R3C1I13 R4C1I17 
R0C2I02 R1C2I06 R2C2I10 R3C2I14 R4C2I18 
R0C3I03 R1C3I07 R2C3I11 R3C3I15 R4C3I19 


-0------------------------------



void input_test(void)
{
    // gpio_set_level(SWM_COL0_GPIO, 1);
    // gpio_set_level(SWM_COL1_GPIO, 0);
    // gpio_set_level(SWM_COL2_GPIO, 0);
    // gpio_set_level(SWM_COL3_GPIO, 0);

    // printf("%d ", gpio_get_level(SWM_ROW0_GPIO));
    // printf("%d ", gpio_get_level(SWM_ROW1_GPIO));
    // printf("%d ", gpio_get_level(SWM_ROW2_GPIO));
    // printf("%d ", gpio_get_level(SWM_ROW3_GPIO));
    // printf("%d\n", gpio_get_level(SWM_ROW4_GPIO));

    for (int rrr = 0; rrr < SW_MATRIX_NUM_ROWS; ++rrr)
    {
        for (int ccc = 0; ccc < SW_MATRIX_NUM_COLS; ++ccc)
        {
            printf("R%dC%dI%d ", rrr, ccc, rowcol_to_index(rrr, ccc));
        }
        printf("\n");
    }

    printf("---------------\n");
}


---------------
R0C0I0 R0C1I1 R0C2I2 R0C3I3 
R1C0I4 R1C1I5 R1C2I6 R1C3I7 
R2C0I8 R2C1I9 R2C2I10 R2C3I11 
R3C0I12 R3C1I13 R3C2I14 R3C3I15 
R4C0I16 R4C1I17 R4C2I18 R4C3I19 



void input_test(void)
{
    // switch_event_t this_event;
    // if (xQueueReceive(switch_event_queue, &this_event, 0) == pdTRUE)
    //  printf("id: %d lvl: %d\n", this_event.id, this_event.level);
}


    // printf("%d\n", gpio_get_level(SD_CARD_DETECT_GPIO));
void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(500)); // brief delay in case of SD card removal reboot
    gpio_install_isr_service(0); // BEFORE GPIO INIT
    my_rotary_encoder_init();
    switch_init();
    oled_init();
    // if (sd_init())
    //     print_nosd();
    neopixel_init();
    vTaskDelay(pdMS_TO_TICKS(500));
    print_nosd();

    ssd1306_SetCursor(20, 40);
    ssd1306_WriteString("haha!!", Font_7x10, White);
    ssd1306_UpdateScreen();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void sw_isr(void * args)
{
    switch_event_t sw_event = {
        .id = (uint8_t)args,
        .level = 1 - gpio_get_level(sw_index_to_gpio_lookup[sw_event.id]),
    };
    uint32_t now = xTaskGetTickCountFromISR();
    sw_status[sw_event.id].level = sw_event.level;

    if(now - sw_status[sw_event.id].last_press_ms <= 33)
        return;
    xQueueSendFromISR(switch_event_queue, &sw_event, NULL);
    sw_status[sw_event.id].last_press_ms = now;
}

void input_test(void)
{
    // UBaseType_t items_in_queue = uxQueueMessagesWaiting(switches_event_queue);
    // printf("queue: %d\n", items_in_queue);

    for (size_t i = 0; i < TOTAL_SW_COUNT; i++)
    {
        printf("%d ", switch_status_all[i]);
    }
    printf("\n");

    if (xQueueReceive(rotary_encoder_event_queue, &event, 0) == pdTRUE)
        printf("Event: id: %d pos: %ld, dir: %d\n", event.state.id, event.state.position, event.state.direction);
    
}

// #define SW_17_U0TX_GPIO GPIO_NUM_43
#define SW_17_SPICS1_GPIO GPIO_NUM_26
#define SW_18_U0RX_GPIO GPIO_NUM_44
// printf("cs1: %d\n", gpio_get_level(SW_17_GPIO));
    // printf("count: %d\n", count);
    // printf("%ld\n", );
// media keys, working
uint8_t keycode[6] = {0x40, 0, 0, 0, 0, 0};
    tud_hid_report(2, keycode, 6);
    vTaskDelay(pdMS_TO_TICKS(100));
    memset(keycode, 0, 6);
    tud_hid_report(2, keycode, 6);

// keyboard keys, working
    ESP_LOGI(TAG, "Sending Keyboard report");
    uint8_t keycode[6] = {HID_KEY_A, 0, 0, 0, 0, 0};
    // the first argument is HID ID?
    tud_hid_keyboard_report(1, 0, keycode);
    vTaskDelay(pdMS_TO_TICKS(100));
    tud_hid_keyboard_report(1, 0, NULL);


static void hid_host_generic_report_callback(const uint8_t *const data, const int length)
{
    printf("@@@@@\n");
    for (int i = 0; i < length; i++) 
    {
        printf("%02X", data[i]);
    }
    putchar('\n');
}



/* Initialize the oled screen */
void ssd1306_init(void) {
    // Init OLED
    ssd1306_write_cmd_byte(0xAE); //display off
    ssd1306_write_cmd_byte(0x20); //Set Memory Addressing Mode
    ssd1306_write_cmd_byte(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                // 10b,Page Addressing Mode (RESET); 11b,Invalid

    ssd1306_write_cmd_byte(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
    ssd1306_write_cmd_byte(0xC0); // Mirror vertically
#else
    ssd1306_write_cmd_byte(0xC8); //Set COM Output Scan Direction
#endif

    ssd1306_write_cmd_byte(0x00); //---set low column address
    ssd1306_write_cmd_byte(0x10); //---set high column address

    ssd1306_write_cmd_byte(0x40); //--set start line address - CHECK

    // ssd1306_SetContrast(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
    ssd1306_write_cmd_byte(0xA0); // Mirror horizontally
#else
    ssd1306_write_cmd_byte(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif


// Set multiplex ratio.
#if (SSD1306_HEIGHT == 128)
    // Found in the Luma Python lib for SH1106.
    ssd1306_write_cmd_byte(0xFF);
#else
    ssd1306_write_cmd_byte(0xA8); //--set multiplex ratio(1 to 64) - CHECK
#endif

#if (SSD1306_HEIGHT == 32)
    ssd1306_write_cmd_byte(0x1F); //
#elif (SSD1306_HEIGHT == 64)
    ssd1306_write_cmd_byte(0x3F); //
#elif (SSD1306_HEIGHT == 128)
    ssd1306_write_cmd_byte(0x3F); // Seems to work for 128px high displays too.
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd1306_write_cmd_byte(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    ssd1306_write_cmd_byte(0xD3); //-set display offset - CHECK
    ssd1306_write_cmd_byte(0x00); //-not offset

    ssd1306_write_cmd_byte(0xD5); //--set display clock divide ratio/oscillator frequency
    ssd1306_write_cmd_byte(0xF0); //--set divide ratio

    ssd1306_write_cmd_byte(0xD9); //--set pre-charge period
    ssd1306_write_cmd_byte(0x22); //

    ssd1306_write_cmd_byte(0xDA); //--set com pins hardware configuration - CHECK
#if (SSD1306_HEIGHT == 32)
    ssd1306_write_cmd_byte(0x02);
#elif (SSD1306_HEIGHT == 64)
    ssd1306_write_cmd_byte(0x12);
#elif (SSD1306_HEIGHT == 128)
    ssd1306_write_cmd_byte(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    ssd1306_write_cmd_byte(0xDB); //--set vcomh
    ssd1306_write_cmd_byte(0x20); //0x20,0.77xVcc

    ssd1306_write_cmd_byte(0x8D); //--set DC-DC enable
    ssd1306_write_cmd_byte(0x14); //
    ssd1306_write_cmd_byte(0xAF); //display on

    // Clear screen
    ssd1306_Fill(Black);

    // Set default values for screen object
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;
}


/* Initialize the oled screen */
void ssd1306_init(void) {
ssd1306_write_cmd_byte(0xAE);  // (p33) 11. display OFF

// set in display function
ssd1306_write_cmd_byte(0x00);  // (p23) 1. set lower column address
ssd1306_write_cmd_byte(0x10);  // (p23) 2. set higher column address

// For SSD1306 - This is Horizontal mode
// For SH1107 - This is page mode (Does not increment rows)
ssd1306_write_cmd_byte(0x20);  // (p24) 3. Set Memory addressing mode

ssd1306_write_cmd_byte(0x81);  // (p28) 4. contrast control
ssd1306_write_cmd_byte(0x2F);  // 0x2F, 0x4F, 0x8F - depending on source

ssd1306_write_cmd_byte(0xA0);  // (p29) 5. set segment remap (L->R | T->B)

ssd1306_write_cmd_byte(0xA8);            // (p30) 6. multiplex ratio
ssd1306_write_cmd_byte(127);  // duty = 1/64 = 0x3F

ssd1306_write_cmd_byte(0xA4);  // (p30) 7. set entire display off/on (0xA4/0xA5)

ssd1306_write_cmd_byte(0xA6);  // (p31) 8. 0xA6/0xA7 = normal (W on Bk) / inverted (Bk on W) display

ssd1306_write_cmd_byte(0xD3);  // (p32) 9. set display offset
ssd1306_write_cmd_byte(0);

ssd1306_write_cmd_byte(0xB0);  // (p33) 12. set page address

ssd1306_write_cmd_byte(0xC0);  // (p34) 13. common output scan direction normal/vertically flipped (0xC0/0xC8)


ssd1306_write_cmd_byte(0xDA);  // set com pins
ssd1306_write_cmd_byte(0x12);

ssd1306_write_cmd_byte(0xD5);  // (p35) 14. set clock divide ratio/OSC frequency
ssd1306_write_cmd_byte(0x50);  // fosc (POR) = 0x50 = 100Hz

ssd1306_write_cmd_byte(0xD9);  // (p36) 15. set discharge/pre-charge period
ssd1306_write_cmd_byte(0x22);  // 0x2* : pre-charge = 2 DCLKs, 0x*2 : discharge = 2 DCLKs

ssd1306_write_cmd_byte(0xDB);  // (p37) 16. set VCOM deselect level
ssd1306_write_cmd_byte(0x40);  // //0x40 | 0x35; Vcomh = 0.43 + 0x35 * 0.006415 * Vref

ssd1306_write_cmd_byte(0xDC);  // (p38) 17. set display start line
ssd1306_write_cmd_byte(0x00);

ssd1306_write_cmd_byte(0xAD);  // (p31) 10. DC-DC control mode
ssd1306_write_cmd_byte(0x8A);  // 0x8A //Set DC-DC enable 1.0SF, DC-DC disabled (external Vpp)
ssd1306_write_cmd_byte(0xAF);  // (p33) 11. display ON

}
    spi_device_acquire_bus(my_spi_handle, portMAX_DELAY);
    ssd1306_init();
    // ssd1306_WriteString("hello", Font_7x10, White);
    ssd1306_UpdateScreen();
    spi_device_release_bus(my_spi_handle);

char* my_data = "hello";
spi_transaction_t my_transaction;

void write_test(void)
{
    ESP_ERROR_CHECK(spi_device_acquire_bus(my_spi_handle, portMAX_DELAY));
    uint8_t tx_size_bytes = strlen(my_data);

    memset(&my_transaction, 0, sizeof(my_transaction));
    my_transaction.length = tx_size_bytes * 8;
    my_transaction.tx_buffer = my_data;
    ESP_ERROR_CHECK(spi_device_polling_transmit(my_spi_handle, &my_transaction));
    spi_device_release_bus(my_spi_handle);
}

    spi_transaction_t t = {
        .length = 5,
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = test,
    };
while(1)
    {
        printf("GPIO%d: %d\n", SW_PLUS, gpio_get_level(SW_PLUS));
        printf("GPIO%d: %d\n", SW_MINUS, gpio_get_level(SW_MINUS));
        printf("GPIO%d: %d\n", RE1_SW, gpio_get_level(RE1_SW));
        printf("GPIO%d: %d\n", RE1_A, gpio_get_level(RE1_A));
        printf("GPIO%d: %d\n", RE1_B, gpio_get_level(RE1_B));
        printf("GPIO%d: %d\n", RE2_SW, gpio_get_level(RE2_SW));
        printf("GPIO%d: %d\n", RE2_A, gpio_get_level(RE2_A));
        printf("GPIO%d: %d\n", RE2_B, gpio_get_level(RE2_B));
        printf("------------\n\n");
        vTaskDelay(pdMS_TO_TICKS(250));
    }
switches
while(1)
    {
        printf("GPIO%d: %d\n", SW_1, gpio_get_level(SW_1));
        printf("GPIO%d: %d\n", SW_2, gpio_get_level(SW_2));
        printf("GPIO%d: %d\n", SW_3, gpio_get_level(SW_3));
        printf("GPIO%d: %d\n", SW_4, gpio_get_level(SW_4));
        printf("GPIO%d: %d\n", SW_5, gpio_get_level(SW_5));
        printf("GPIO%d: %d\n", SW_6, gpio_get_level(SW_6));
        printf("GPIO%d: %d\n", SW_7, gpio_get_level(SW_7));
        printf("GPIO%d: %d\n", SW_8, gpio_get_level(SW_8));
        printf("GPIO%d: %d\n", SW_9, gpio_get_level(SW_9));
        printf("GPIO%d: %d\n", SW_10, gpio_get_level(SW_10));
        printf("GPIO%d: %d\n", SW_11, gpio_get_level(SW_11));
        printf("GPIO%d: %d\n", SW_12, gpio_get_level(SW_12));
        printf("GPIO%d: %d\n", SW_13, gpio_get_level(SW_13));
        printf("GPIO%d: %d\n", SW_14, gpio_get_level(SW_14));
        printf("GPIO%d: %d\n", SW_15, gpio_get_level(SW_15));
        printf("GPIO%d: %d\n", SW_16, gpio_get_level(SW_16));
        printf("GPIO%d: %d\n", SW_17, gpio_get_level(SW_17));
        // printf("GPIO%d: %d\n", SW_18, gpio_get_level(SW_18));
        printf("GPIO%d: %d\n", SW_19_U0RX, gpio_get_level(SW_19_U0RX));
        printf("GPIO%d: %d\n", SW_20, gpio_get_level(SW_20));
        printf("------------\n\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

#define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
static const char *TAG = "example";

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID report descriptor
 *
 * In this example we implement Keyboard + Mouse HID device,
 * so we must define both report descriptors
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD) ),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE) )
};

/**
 * @brief String descriptor
 */
const char* hid_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "TinyUSB",             // 1: Manufacturer
    "TinyUSB Device",      // 2: Product
    "123456",              // 3: Serials, should use chip ID
    "Example HID interface",  // 4: HID
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

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
}

/********* Application ***************/

typedef enum {
    MOUSE_DIR_RIGHT,
    MOUSE_DIR_DOWN,
    MOUSE_DIR_LEFT,
    MOUSE_DIR_UP,
    MOUSE_DIR_MAX,
} mouse_dir_t;

#define DISTANCE_MAX        125
#define DELTA_SCALAR        5

static void mouse_draw_square_next_delta(int8_t *delta_x_ret, int8_t *delta_y_ret)
{
    static mouse_dir_t cur_dir = MOUSE_DIR_RIGHT;
    static uint32_t distance = 0;

    // Calculate next delta
    if (cur_dir == MOUSE_DIR_RIGHT) {
        *delta_x_ret = DELTA_SCALAR;
        *delta_y_ret = 0;
    } else if (cur_dir == MOUSE_DIR_DOWN) {
        *delta_x_ret = 0;
        *delta_y_ret = DELTA_SCALAR;
    } else if (cur_dir == MOUSE_DIR_LEFT) {
        *delta_x_ret = -DELTA_SCALAR;
        *delta_y_ret = 0;
    } else if (cur_dir == MOUSE_DIR_UP) {
        *delta_x_ret = 0;
        *delta_y_ret = -DELTA_SCALAR;
    }

    // Update cumulative distance for current direction
    distance += DELTA_SCALAR;
    // Check if we need to change direction
    if (distance >= DISTANCE_MAX) {
        distance = 0;
        cur_dir++;
        if (cur_dir == MOUSE_DIR_MAX) {
            cur_dir = 0;
        }
    }
}

static void app_send_hid_demo(void)
{
    // Keyboard output: Send key 'a/A' pressed and released
    ESP_LOGI(TAG, "Sending Keyboard report");
    uint8_t keycode[6] = {HID_KEY_A};
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, keycode);
    vTaskDelay(pdMS_TO_TICKS(50));
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);

    // Mouse output: Move mouse cursor in square trajectory
    ESP_LOGI(TAG, "Sending Mouse report");
    int8_t delta_x;
    int8_t delta_y;
    for (int i = 0; i < (DISTANCE_MAX / DELTA_SCALAR) * 4; i++) {
        // Get the next x and y delta in the draw square pattern
        mouse_draw_square_next_delta(&delta_x, &delta_y);
        tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, delta_x, delta_y, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void app_main(void)
{
    // Initialize button that will trigger HID reports
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(APP_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");

    while (1) {
        if (tud_mounted()) {
            static bool send_hid_data = true;
            if (send_hid_data) {
                app_send_hid_demo();
            }
            send_hid_data = !gpio_get_level(APP_BUTTON);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
