#include "loop.h"

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

void deadlock(struct k_sem *a,
             struct k_sem *b,
             int *counter)
{
    struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    k_sem_take(a, K_FOREVER);
    {
        k_timer_start(&timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
        k_timer_status_sync(&timer);
        k_sem_take(b, K_FOREVER);
        {
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

int nth_fibonacci(int element)
{
    if (element == 0) {
        return 0;
    } else if (element == 1) {
        return 1;
    } else {
        int n, n_1, n_2;
        n = 1;
        n_1 = 1;
        n_2 = 0;
        for (int i = 1; i < element; i++) {
            n = n_1 + n_2;
            n_2 = n_1;
            n_1 = n;
        }
        return n;
    }
}

void fibonacci_worker(void *element, void *result)
{
    int nth = *((int *)element);
    int *output = (int *)result;
    *output = nth_fibonacci(nth);
}

void fibonacci_supervisor(void *p, void *a)
{
    struct thread_pool *pool = (struct thread_pool *)p;
    struct fibonacci *args = (struct fibonacci *)a;

    for (int i = 0; i < args->size; i++) {
        k_thread_create(pool->threads[i],
                        pool->stacks[i],
                        pool->stacksize,
                        (k_thread_entry_t) fibonacci_worker,
                        (void *)(&(args->elements[i])),
                        (void *)(&(args->results[i])),
                        NULL,
                        K_PRIO_COOP(7),
                        0,
                        K_NO_WAIT);
    }

    for (int i = 0; i < args->size; i++) {
        k_thread_join(pool->threads[i], K_FOREVER);
    }
}

int async_fibonacci(struct k_thread *supervisor_thread, k_thread_stack_t *supervisor_stack, int stacksize,
                    struct thread_pool *pool, struct fibonacci *args)
{
    if (pool->size != args->size) return 1;
    k_thread_create(supervisor_thread,
                    supervisor_stack,
                    stacksize,
                    (k_thread_entry_t) fibonacci_supervisor,
                    pool,
                    args,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);
    k_thread_join(supervisor_thread, K_FOREVER);
    return 0;
}
