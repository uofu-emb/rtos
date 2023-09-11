#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <stdio.h>
#include "uart_hello.h"

#define UART DT_NODELABEL(usart1)

void main(void)
{
    const struct device *dev = device_get_binding(DT_LABEL(UART));
    echo_uppercase(dev);
}
