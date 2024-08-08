void delay_ms(int ms)
{
  int x = ms << 16;
  while (x--);
}

void start_thread(void (* fun)(void))
{
  /* fun(); */
}

