#include <lab1.h>
#define MAIN_DELAY 1000
#define THREAD_DELAY 500

int toggle_thread_setup(struct gpio *dev, unsigned int* counter)
{
  *counter = 1;
  dev->CR |= 1 << 2;
  return 0;
}

int toggle_thread_iteration(const unsigned int led_is_on, struct gpio *dev, unsigned int* counter)
{
  *counter = *counter + 1;
  if (led_is_on) {
    dev->DR &= ~(1<<2);
  } else {
    dev->DR |= 1<<2;
  }
  
  delay_ms(THREAD_DELAY);
  return !led_is_on;
}

int main_thread_setup(struct gpio *dev)
{
  dev->CR |= 1 << 3;
  return 1;
}

int main_thread_iteration(const unsigned int led_is_on, struct gpio *dev)
{
  if (led_is_on) {
    dev->DR |= 1<<2;
  } else {
    dev->DR &= ~(1<<2);
  }
  delay_ms(MAIN_DELAY);
  return !led_is_on;
}

