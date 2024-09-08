#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void expand_mmov(int16_t xtotal, int16_t ytotal)
{
  int16_t xsign = (xtotal < 0) ? -1 : 1;
  int16_t ysign = (ytotal < 0) ? -1 : 1;

  xtotal = abs(xtotal);
  ytotal = abs(ytotal);

  uint8_t loops_needed_x = abs(xtotal) / 128;
  uint8_t loops_needed_y = abs(ytotal) / 128;

  uint8_t total_loops_needed = MAX(loops_needed_x, loops_needed_y);
  printf("lx:%d ly:%d tl: %d\n", loops_needed_x, loops_needed_y, total_loops_needed);

  for (int i = 0; i <= total_loops_needed; ++i)
  {
    int8_t this_x_amount;
    if(xtotal > 127)
      this_x_amount = 127;
    else
      this_x_amount = xtotal;
    xtotal -= this_x_amount;

    int8_t this_y_amount;
    if(ytotal > 127)
      this_y_amount = 127;
    else
      this_y_amount = ytotal;
    ytotal -= this_y_amount;

    printf("mmov %d %d\n", this_x_amount*xsign, this_y_amount*ysign);
  }
}

int main(int argc, char *argv[])
{
  expand_mmov(-400, -69);
}

