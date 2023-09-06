#ifndef _BLINK_H_
#define _BLINK_H_
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

int second_thread_setup(const struct device *dev, gpio_pin_t pin, gpio_flags_t flags, int *counter);

int second_thread_iteration(const struct device *dev, gpio_pin_t pin, int led_state, int *counter);

int main_thread_setup(const struct device *dev, gpio_pin_t pin, gpio_flags_t flags);

int main_thread_iteration(const struct device *dev, gpio_pin_t pin, int led_state);

#endif
