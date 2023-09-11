#pragma once

#include <drivers/uart.h>
void echo_uppercase_ptr(const struct device *dev,
                        int (*uart_in)(const struct device *, unsigned char *),
                        void (*uart_out)(const struct device *, unsigned char));
