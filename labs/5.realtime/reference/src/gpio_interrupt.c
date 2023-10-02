#include <zephyr.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#define DEV_IN DT_NODELABEL(gpioa)
#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0
#define PIN_IN 1

struct gpio_callback callback;

void pin_interrupt(const struct device *port,
                   struct gpio_callback *cb,
                   gpio_port_pins_t pins_)
{
    gpio_pin_toggle(port, PIN_OUT);
}

void interrupt_main(void)
{
	const struct device *dev_in, *dev_out;
    dev_in = device_get_binding(DT_LABEL(DEV_IN));
    dev_out = device_get_binding(DT_LABEL(DEV_OUT));

    gpio_pin_configure(dev_out, PIN_OUT, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure(dev_in, PIN_IN, GPIO_INPUT);
    if (!gpio_pin_interrupt_configure(dev_in, PIN_IN, GPIO_INT_EDGE_RISING))
        return;
    gpio_init_callback(&callback, pin_interrupt, 1 << PIN_IN);
    k_sleep(K_FOREVER);
}
