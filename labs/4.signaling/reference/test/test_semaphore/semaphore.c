#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>

#define STACKSIZE 2000

struct k_thread coop_thread;
K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);

struct k_sem response;
struct k_sem request;
int input;
int output;

void handle_calculation(void)
{
    printf("+ Waiting for request\n");
    k_sem_take(&request, K_FOREVER);
    printf("+ Handling calculation\n");
    output = input + 5;
    k_sleep(K_MSEC(1));
    printf("+ Done with calculation\n");
    k_sem_give(&response);
}

int request_calculate(int value)
{
    input = value;
    printf("- Handoff to worker\n");
    k_sem_give(&request);
    printf("- Waiting for results\n");
    k_sem_take(&response, K_FOREVER);
    printf("- Result ready\n");
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
    printf("Starting worker thread\n");
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
    for (int counter = 0; counter < 8; counter++) {
        k_sleep(K_MSEC(5000));
        int result = request_calculate(counter);
        printf("- Got result %d for %d\n", result, counter);
	}

	return 0;
}
