#include "loop.h"

#define SLEEPTIME 5

int do_loop(struct k_timer *timer,
            struct k_sem *semaphore,
            int *counter,
            char *src,
            k_timeout_t timeout)
{
    if (k_sem_take(semaphore, timeout))
        return 1;
    {
        (*counter)++;
        printk("hello world from %s! Count %d\n", src, *counter);
    }
    k_sem_give(semaphore);
    k_timer_start(timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
    k_timer_status_sync(timer);
    return 0;
}

void deadlock(struct k_sem *a, struct k_sem *b, int *counter)
{
    printk("inside deadlock %d\n", *counter);
    (*counter)++;
    struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    k_sem_take(a, K_FOREVER);
    {
        (*counter)++;
        printk("\tinside first lock %d\n", *counter);
        k_yield();
        printk("\tpost-yield %d\n", *counter);
        k_timer_start(&timer, K_MSEC(10), K_NO_WAIT);
        k_timer_status_sync(&timer);
        k_sem_take(b, K_FOREVER);
        {
            printk("\tinside second lock %d\n", *counter);
            (*counter)++;
        }
        k_sem_give(b);
    }
    k_sem_give(a);
}

int orphaned_lock(struct k_sem *semaphore, k_timeout_t timeout, int *counter)
{
    if (k_sem_take(semaphore, timeout)) return 1;
    (*counter)++;
    if (*counter % 2) {
        return 0;
    }
    printk("Count %d\n", *counter);
    k_sem_give(semaphore);
    return 0;
}

int unorphaned_lock(struct k_sem *semaphore, k_timeout_t timeout, int *counter)
{
    if (k_sem_take(semaphore, timeout))
        return 1;
    {
        (*counter)++;
        if (!(*counter % 2)) {
            printk("Count %d\n", *counter);
        }
    }
    k_sem_give(semaphore);
    return 0;
}
