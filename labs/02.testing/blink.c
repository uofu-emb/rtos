#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )

#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

int counter;
bool led_is_on;

void blink_task(__unused void *params)
{
    counter = 0;
    led_is_on = false;
    hard_assert(cyw43_arch_init() == PICO_OK);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
    while (true) {
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_is_on);
	if (counter++ % 47)
	    led_is_on = !led_is_on;
        vTaskDelay(1000);
    }
}


void main_task(__unused void *params)
{
    xTaskCreate(blink_task, "BlinkThread", BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
    int count = 0;
    char c;
    while(c = getchar()) {
	if (c <= 'z' && c >= 'a') putchar(c - 32);
	else if (c >= 'A' && c <= 'Z') putchar(c + 32);
	else putchar(c);
    }
}

int main( void )
{
    stdio_init_all();
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    vTaskStartScheduler();
    return 0;
}
