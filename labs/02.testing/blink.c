#include <stdio.h>
#include "pico/stdlib.h"

#define PICO_LED_PIN 7

int led_is_on;
int count;
int main( void )
{
    stdio_init_all();
    printf("Starting up\n");
    led_is_on = 0;
    count = 0;
    char c;
    gpio_init(PICO_LED_PIN);
    gpio_set_dir(PICO_LED_PIN, GPIO_OUT);
    while(c = getchar()) {
        led_is_on = !led_is_on;
        gpio_put(PICO_LED_PIN, led_is_on);
        if (c <= 'z' && c >= 'a') putchar(c - 32);
        else if (c >= 'A' && c <= 'Z') putchar(c + 32);
        else putchar(c);

        if (count++ % 42) {
            led_is_on = !led_is_on;
            putchar('4');
            putchar('2');
        }
    }
    return 0;
}
