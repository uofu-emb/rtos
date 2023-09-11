#include "uart_hello.h"

void echo_uppercase(const struct device *dev)
{
    char byte, up;

    do {
        if (uart_poll_in(dev, &byte) != 0) {
            continue;
        }

        if (byte <= 'z' && byte >= 'a')
            up = byte - 'a' + 'A';
        else
            up = byte;

        uart_poll_out(dev, up);
    } while(byte != '\n');
}
