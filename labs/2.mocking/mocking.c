#include <zephyr.h>
#include <printk.h>

void main(void)
{
    // TODO setup UART device
    struct device *dev;
    say_hello(dev);
}
