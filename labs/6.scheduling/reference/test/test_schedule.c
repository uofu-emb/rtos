#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

#define STACKSIZE 2000

K_THREAD_STACK_DEFINE(primary_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(secondary_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(super_stack, STACKSIZE);
struct k_thread primary_thread, secondary_thread, super_thread;

void busy_busy(char *name)
{
    printk("start busy_busy %s\n", name);

    for (int i = 0; ; i++);
}

void busy_yield(char *name)
{
    printk("start busy_yield %s\n", name);
    for (int i = 0; ; i++) {
        if (!i % 72000) {
            printk("yield %s\n", name);
            k_yield();
            printk("back %s\n", name);
        }
    }
}

void priority_inversion(char *name, struct k_sem *sem)
{
    printk("start priority inversion %s\n", name);
    k_sem_take(sem, K_FOREVER);
    printk("got semaphore %s\n", name);
    for (int i = 0; ; i++);
}

void super_entry(struct k_thread *primary, struct k_thread *secondary)
{
    printk("super is suspending threads\n");
    k_thread_suspend(primary);
    k_thread_suspend(secondary);
}

char *primary_name = "primary";
char *secondary_name = "secondary";

void run_analyzer(k_thread_entry_t thread_entry,
                  void *arg,
                  int pri_prio,  k_timeout_t pri_delay,
                  k_thread_runtime_stats_t *pri_stat,
                  int sec_prio,  k_timeout_t sec_delay,
                  k_thread_runtime_stats_t *sec_stat,
                  k_thread_runtime_stats_t *total_stat)
{
    k_thread_runtime_stats_t start_stats, pri_stats, sec_stats, total_stats;

    /* k_thread_runtime_stats_all_get(&start_stats); */
    timing_t start = start_stats.execution_cycles;
    // Meta-IRQ is enabled, forcing this to always preempt.
    int super_prio = -CONFIG_NUM_COOP_PRIORITIES;
    k_thread_create(&super_thread,
                    super_stack,
                    STACKSIZE,
                    (k_thread_entry_t) super_entry,
                    &primary_thread,
                    &secondary_thread,
                    NULL,
                    super_prio,
                    0,
                    K_MSEC(5000));
    printk("create primary\n");
    k_thread_create(&primary_thread,
                    primary_stack,
                    STACKSIZE,
                    thread_entry,
                    primary_name,
                    arg,
                    NULL,
                    pri_prio,
                    0,
                    pri_delay);
    printk("create secondary\n");
    k_thread_create(&secondary_thread,
                    secondary_stack,
                    STACKSIZE,
                    thread_entry,
                    secondary_name,
                    arg,
                    NULL,
                    sec_prio,
                    0,
                    sec_delay);
    k_thread_join(&super_thread, K_MSEC(5500));
    printk("super joined\n");
    k_thread_runtime_stats_get(&primary_thread, &pri_stats);
    timing_t primary = pri_stats.execution_cycles;
    k_thread_runtime_stats_get(&secondary_thread, &sec_stats);
    timing_t secondary = sec_stats.execution_cycles;
    /* k_thread_runtime_stats_all_get(&total_stats); */
    timing_t total = total_stats.execution_cycles;
    timing_t elapsed = total - start;
    printf("primary %lu secondary %lu start %lu end %lu elapsed %lu (raw)\n",
           primary, secondary, start, total, elapsed);
    /*        arch_timing_cycles_to_ns(pri_stats->execution_cycles), */
    /*        arch_timing_cycles_to_ns(sec_stats->execution_cycles), */
    /*        arch_timing_cycles_to_ns(total_stats->execution_cycles)); */
    k_thread_abort(&primary_thread);
    k_thread_abort(&secondary_thread);
}

void test_priority_inversion(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    struct k_sem sem;
    k_sem_init(&sem, 1, 1);

    run_analyzer((k_thread_entry_t)priority_inversion, &sem,
                 K_PRIO_PREEMPT(2), K_MSEC(1), &pri_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(2), &sec_stats,
                 &total_stats);
}

void test_coop__no_priority__no_yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(3), K_MSEC(1), &pri_stats,
                 K_PRIO_COOP(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}

void test_preempt__no_priority__no_yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(1), &pri_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}

void test_coop__no_priority__yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(1), &pri_stats,
                 K_PRIO_COOP(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}
void test_preempt__no_priority__yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(1), &pri_stats,
                 K_PRIO_COOP(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}

void test_coop__priority_low_first__no_yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(2), K_MSEC(1), &pri_stats,
                 K_PRIO_COOP(3), K_MSEC(2), &sec_stats,
                 &total_stats);
}
void test_coop__priority_high_first__no_yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(2), K_MSEC(2), &pri_stats,
                 K_PRIO_COOP(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}

void test_coop__priority_low_first__yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(2), K_MSEC(1), &pri_stats,
                 K_PRIO_COOP(3), K_MSEC(2), &sec_stats,
                 &total_stats);
}
void test_coop__priority_high_first__yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(2), K_MSEC(2), &pri_stats,
                 K_PRIO_COOP(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}

void test_preempt__priority__no_yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_PREEMPT(2), K_MSEC(1), &pri_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}
void test_preempt__priority__yield(void)
{
    struct k_thread_runtime_stats pri_stats, sec_stats, total_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_PREEMPT(2), K_MSEC(1), &pri_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(1), &sec_stats,
                 &total_stats);
}


int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_priority_inversion);
    // RUN_TEST(test_overflow);

    RUN_TEST(test_coop__no_priority__no_yield);
    RUN_TEST(test_preempt__no_priority__no_yield);

    RUN_TEST(test_coop__no_priority__yield);
    RUN_TEST(test_preempt__no_priority__yield);

    RUN_TEST(test_coop__priority_low_first__no_yield);
    RUN_TEST(test_coop__priority_high_first__no_yield);

    RUN_TEST(test_coop__priority_low_first__yield);
    RUN_TEST(test_coop__priority_high_first__yield);

    RUN_TEST(test_preempt__priority__no_yield);
    RUN_TEST(test_preempt__priority__yield);
    return UNITY_END();
}
