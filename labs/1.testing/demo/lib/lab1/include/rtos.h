#ifndef _RTOS_H_
#define _RTOS_H_

struct gpio {
  unsigned int CR;
  unsigned int DR;
};

void delay_ms(int ms);
void start_thread(void (* fun)(void));
#endif
