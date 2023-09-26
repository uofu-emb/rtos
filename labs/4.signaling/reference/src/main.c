#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>

#define STACKSIZE 2000
#define SLEEPTIME 1000

struct k_thread coop_thread;
K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);

struct k_sem response;
struct k_sem request;
int input;
int output;

void handle_calculation(void)
{
    k_sem_take(&request, K_FOREVER);
    printf("Handling calculation");
    output = input + 5;
    k_sem_give(&response);
    printf("Done with calculation");
    k_sem_give(&request);
    k_yield();
    k_sem_take(&response, K_FOREVER);
}

void request_async_calculate(int value)
{
    input = value;
    printf("Handoff to worker");
    k_sem_give(&request);
    k_yield();
}

int block_for_result(void)
{
    printf("Waiting for results");
    k_sem_take(&response, K_FOREVER);
    k_sem_take(&request, K_FOREVER);
    printf("Got result");
    k_sem_give(&response);
    return output;
}

void thread_entry(void)
{
	while (1) {
        handle_calculation();
 	}
}

int main(void)
{
    k_sem_init(&request, 0, 1);
    k_sem_init(&response, 0, 1);
    printf("Starting worker thread");
    k_thread_create(&coop_thread,
                    coop_stack,
                    STACKSIZE,
                    (k_thread_entry_t) thread_entry,
                    NULL,
                    NULL,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    int counter = 0;
    while (1) {
        printf("Requesting calculation");
        request_async_calculate(counter);
        int result = block_for_result();
        printf("Got result %d for %d", result, ++counter);
        k_timer_start(&timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
        k_timer_status_sync(&timer);
	}

	return 0;
}
