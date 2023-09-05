#ifndef _LAB1_H_
#define _LAB1_H_
#include <rtos.h>

int toggle_thread_setup(struct gpio *dev, unsigned int* counter);
int toggle_thread_iteration(const unsigned int led_is_on, struct gpio *dev, unsigned int* counter);
int main_thread_setup(struct gpio *dev);
int main_thread_iteration(const unsigned int led_is_on, struct gpio *dev);
#endif
