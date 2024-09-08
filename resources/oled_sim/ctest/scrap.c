uint16_t test = 65535;
  printf("result: %d\n", (int16_t)test*2);

void expand_mmov(int16_t xtotal, int16_t ytotal)
{
  int16_t xsign = (xtotal < 0) ? -1 : 1;
  int16_t ysign = (ytotal < 0) ? -1 : 1;
  xtotal = abs(xtotal);
  ytotal = abs(ytotal);
  while(xtotal > 0 || ytotal > 0)
  {
    int16_t this_step_x = 127
    if xtotal < 127:
      this_step_x = xtotal
  }
}


    // printf("this_x_amount:%d xtotal:%d\n", this_x_amount, xtotal);

    // printf("x:%d y:%d\n", xtotal-i*128, ytotal);