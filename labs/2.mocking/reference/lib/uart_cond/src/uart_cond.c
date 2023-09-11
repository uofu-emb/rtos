#include "uart_cond.h"

void echo_uppercase_conditional(const struct device *dev)
{
    char byte, up;

    do {
        #ifndef TESTING_ENV
        if (uart_poll_in(dev, &byte) != 0) {
            continue;
        }
        #else
        test_uart_in(&byte);
        #endif

        if (byte <= 'z' && byte >= 'a')
            up = byte - 'a' + 'A';
        else
            up = byte;
        #ifndef TESTING_ENV
        uart_poll_out(dev, up);
        #else
        test_uart_out(up);
        #endif
    } while(byte != '\n');
}
