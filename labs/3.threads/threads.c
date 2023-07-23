#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>

#define STACKSIZE 2000
#define SLEEPTIME 1000

struct k_thread coop_thread;
K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);

struct k_sem semaphore;
int counter;
void thread_entry(void)
{
	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);

	while (1) {
        counter = counter + 1;
		printk("hello world from %s! Count %d\n", "thread", counter);
		k_timer_start(&timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
		k_timer_status_sync(&timer);
	}
}

int main(void)
{
    counter = 0;
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

	while (1) {
        k_sem_take(&semaphore, K_FOREVER);
        counter = counter + 1;
		printk("hello world from %s! Count %d\n", "main", counter);
		k_timer_start(&timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
		k_timer_status_sync(&timer);
        k_sem_give(&semaphore);
	}

	return 0;
}
