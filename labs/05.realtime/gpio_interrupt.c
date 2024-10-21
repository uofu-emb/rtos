#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/sync.h>

int toggle = 1;
void irq_callback(uint gpio, uint32_t event_mask)
{
    if (gpio != IN_PIN) return;
    toggle = !toggle;
    gpio_put(OUT_PIN, toggle);
}

int main(void)
{
    stdio_init_all();
    gpio_init(IN_PIN);
    gpio_init(OUT_PIN);
    gpio_put(OUT_PIN, toggle);
    gpio_set_input_enabled(IN_PIN, true);
    gpio_set_irq_enabled_with_callback(IN_PIN, GPIO_IRQ_EDGE_RISE, true, irq_callback);
    while(1) __wfi();
    return 0;
}
