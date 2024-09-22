
// ------------- pong ----------

// top left is 0,0
// bottom right is 127, 127
#define BALL_RADIUS 2
#define PADDLE_WIDTH 2
#define PADDLE_HEIGHT 32
#define PADDLE_MOVEMENT_STEP 7

int16_t ball_pos_x;
int16_t ball_pos_y;
int16_t ball_velocity_x;
int16_t ball_velocity_y;
int16_t paddle_pos;

void update_ball_pos(void)
{
  ball_pos_x += ball_velocity_x;
  ball_pos_y += ball_velocity_y;

  // top and bottom wall
  if((ball_pos_y >= 127 - BALL_RADIUS*2) || (ball_pos_y <= BALL_RADIUS*2))
    ball_velocity_y = ball_velocity_y * -1;
  // right wall
  if((ball_pos_x >= 127 - BALL_RADIUS*2))
    ball_velocity_x = ball_velocity_x * -1;
  // paddle collision check
  if(ball_pos_x <= BALL_RADIUS*2)
  {
    int16_t paddle_top = paddle_pos - PADDLE_HEIGHT/2;
    int16_t paddle_bottom = paddle_pos + PADDLE_HEIGHT/2;
    // contact with paddle
    if(ball_pos_y >= paddle_top && ball_pos_y <= paddle_bottom)
    {
      ball_velocity_x = ball_velocity_x * -1;
    }
    else
    {
      ball_velocity_x = 0;
      ball_velocity_y = 0;
    }
  }
}

void draw_court(void)
{
  ssd1306_DrawRectangle(0,0,127,127,White);
  ssd1306_Line(0,0,0,127,Black);
}

void draw_ball(void)
{
  uint8_t drawx = (uint8_t)ball_pos_x;
  uint8_t drawy = (uint8_t)ball_pos_y;
  if(drawx >= 127 - BALL_RADIUS || drawx <= BALL_RADIUS)
    return;
  if(drawy >= 127 - BALL_RADIUS || drawy <= BALL_RADIUS)
    return;
  ssd1306_FillCircle(drawx, drawy, 2, White);
}

void update_paddle_position(uint8_t active_key)
{
  if(active_key == RE1_CW+1)
  {
    paddle_pos += PADDLE_MOVEMENT_STEP;
    if(paddle_pos >= (127-PADDLE_HEIGHT/2))
      paddle_pos = (127-PADDLE_HEIGHT/2);
    return;
  }
  if(active_key == RE1_CCW+1)
  {
    paddle_pos -= PADDLE_MOVEMENT_STEP;
    if(paddle_pos <= PADDLE_HEIGHT/2)
      paddle_pos = PADDLE_HEIGHT/2;
  }
}

void draw_paddle(void)
{
  int16_t paddle_top_y = paddle_pos - PADDLE_HEIGHT/2;
  if(paddle_top_y < 0)
    paddle_top_y = 0;
  int16_t paddle_bottom_y = paddle_pos + PADDLE_HEIGHT/2;
  if(paddle_bottom_y > 127)
    paddle_bottom_y = 127;
  ssd1306_FillRectangle(0, paddle_top_y, PADDLE_WIDTH, paddle_bottom_y, White);
}

void pong_test(void)
{
  ball_pos_x = 20;
  ball_pos_y = 30;
  ball_velocity_x = 2;
  ball_velocity_y = 3;
  paddle_pos = 64;
  while(1)
  {
    uint8_t active_key = readkey_nonblocking_1_indexed();
    update_paddle_position(active_key);

    // printf("%d\n", paddle_pos);

    // calculation
    update_ball_pos();

    // ---- drawing ---------
    ssd1306_Fill(Black);
    draw_paddle();
    draw_court();
    draw_ball(); 
    ssd1306_UpdateScreen();

    // ------ frame wait ------
    delay_ms(33);
  }
}

/*
dont forget to plus 1!
#define RE1_CW 20
#define RE1_CCW 21
#define RE2_CW 23
#define RE2_CCW 24
*/

// ------------- pong ----------