#include <stdio.h>
#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include "loop.h"

struct k_sem semaphore;
int counter;

#define SLEEPTIME 1000
#define THREAD_COUNT 16
#define STACKSIZE 100
K_THREAD_STACK_ARRAY_DEFINE(coop_stack, THREAD_COUNT, STACKSIZE);

void thread_entry(int number)
{
	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    char buf[32];
    snprintf(buf, 32, "thread %d", number);
    do_loop(&timer, &semaphore, &counter, buf, K_FOREVER);
}

int main(void)
{
    counter = 0;
    k_sem_init(&semaphore, 1, 1);

    struct k_thread coop_thread[THREAD_COUNT];


	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    while (1) {
        k_timer_start(&timer, K_MSEC(SLEEPTIME/2), K_NO_WAIT);
        k_timer_status_sync(&timer);
        for (int i = 0; i < THREAD_COUNT; i++) {
            k_thread_create(&coop_thread[i],
                            coop_stack[i],
                            STACKSIZE,
                            (k_thread_entry_t) thread_entry,
                            (void *)i,
                            NULL,
                            NULL,
                            K_PRIO_COOP(7),
                            0,
                            K_NO_WAIT);
        }
        do_loop(&timer, &semaphore, &counter, "main", K_FOREVER);
    }
	return 0;
}
