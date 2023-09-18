#pragma once

#include <zephyr.h>
#include <kernel.h>
#include <arch/cpu.h>
#include <sys/printk.h>

#define SLEEPTIME 50

int do_loop(struct k_timer *timer,
            struct k_sem *semaphore,
            int *counter,
            char *src,
            k_timeout_t timeout);

struct thread_pool {
    struct k_thread **threads;
    k_thread_stack_t **stacks;
    int size;
    int stacksize;
};

struct fibonacci {
    const int *elements;
    int *results;
    int size;
};

int nth_fibonacci(int element);

int async_fibonacci(struct k_thread *supervisor_thread, k_thread_stack_t *supervisor_stack, int stacksize,
                    struct thread_pool *pool, struct fibonacci *arg);
int unorphaned_lock(struct k_sem *semaphore, k_timeout_t timeout, int *counter);
int orphaned_lock(struct k_sem *semaphore, k_timeout_t timeout, int *counter);

void deadlock(struct k_sem *a, struct k_sem *b, int *counter);
