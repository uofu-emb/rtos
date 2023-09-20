#pragma once

#include <zephyr.h>
#include <kernel.h>
#include <arch/cpu.h>
#include <sys/printk.h>

int do_loop(struct k_timer *timer,
            struct k_sem *semaphore,
            int *counter,
            char *src,
            k_timeout_t timeout);

int unorphaned_lock(struct k_sem *semaphore, k_timeout_t timeout, int *counter);
int orphaned_lock(struct k_sem *semaphore, k_timeout_t timeout, int *counter);
void deadlock(struct k_sem *a, struct k_sem *b, int *counter);
