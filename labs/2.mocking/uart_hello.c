#include <zephyr/drivers/uart.h>

int count = 0;
void say_hello_conditional(const struct device *dev)
{
    char *c = greeting;
    while (*c) {
        uart_poll_out(dev, *c++);
    }
    count++;
}

int how_many_greeted(void)
{
    return count;
}

void echo_conditional(const struct device *dev)
{
    char byte;
    uart_poll_in(dev, &byte);
    while(byte != '\n') {
        uart_poll_out(dev, byte);
        uart_poll_in(dev, &byte);
    }
}
