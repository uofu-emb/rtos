#pragma once

#ifdef TESTING_ENV
struct device {};
#else
#include <drivers/uart.h>
#endif

void echo_uppercase(const struct device *dev);
