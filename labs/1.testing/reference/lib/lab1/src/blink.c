#include "blink.h"

int second_thread_setup(const struct device *dev, gpio_pin_t pin, gpio_flags_t flags, int *counter)
{
    counter = 0;
	gpio_pin_configure(dev, pin, GPIO_OUTPUT_ACTIVE | flags);
    return 0;
}

int second_thread_iteration(const struct device *dev, gpio_pin_t pin, int led_state, int *counter)
{
    *counter = *counter + 1;
    led_state = !led_state;
    gpio_pin_set(dev, pin, (int)led_state);
    return led_state;
}

int main_thread_setup(const struct device *dev, gpio_pin_t pin, gpio_flags_t flags)
{
    gpio_pin_configure(dev, pin,  GPIO_OUTPUT_ACTIVE | flags);
    return 0;
}

int main_thread_iteration(const struct device *dev, gpio_pin_t pin, int led_state)
{
    led_state = !led_state;
    gpio_pin_set(dev, pin, (int)led_state);
    return led_state;
}
