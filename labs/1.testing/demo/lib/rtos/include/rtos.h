#ifndef _RTOS_H_
#define _RTOS_H_

#define GPIOA ((struct gpio*)0x10000)
#define GPIOB ((struct gpio*)0x10013)
;

struct gpio {
  unsigned int CR;
  unsigned int DR;
};

void delay_ms(int ms);
void start_thread(void (* fun)(void));
#endif
