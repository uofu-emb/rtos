#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>
#include <drivers/uart.h>

void setUp(void) {}

void tearDown(void) {}

#define STACKSIZE 2000
extern k_thread_runtime_stats_t threads_runtime_stats;
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
        if (!(i & 0xFF)) {
            k_yield();
        }
    }
}

void busy_sleep(char *name)
{
    printk("start busy_busy %s\n", name);
    k_busy_wait(10000);
    k_sleep(K_MSEC(490));
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

void run_analyzer_split(k_thread_entry_t pri_thread_entry,
                        void *pri_arg0, void *pri_arg1, void *pri_arg2,
                        int pri_prio,  k_timeout_t pri_delay,
                        uint64_t *pri_duration,

                        k_thread_entry_t sec_thread_entry,
                        void *sec_arg0, void *sec_arg1, void *sec_arg2,
                        int sec_prio,  k_timeout_t sec_delay,
                        uint64_t *sec_duration,

                        uint64_t *total_duration)
{
	uint64_t start, primary, secondary, end, elapsed;
    k_thread_runtime_stats_t pri_stats, sec_stats, start_stats, end_stats;;

    k_thread_runtime_stats_all_get(&start_stats);

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
                    pri_thread_entry,
                    pri_arg0,
                    pri_arg1,
                    pri_arg2,
                    pri_prio,
                    0,
                    pri_delay);
    printk("create secondary\n");
    k_thread_create(&secondary_thread,
                    secondary_stack,
                    STACKSIZE,
                    sec_thread_entry,
                    sec_arg0,
                    sec_arg1,
                    sec_arg2,
                    sec_prio,
                    0,
                    sec_delay);
    k_thread_join(&super_thread, K_MSEC(5500));
    printk("super joined\n");

    k_thread_runtime_stats_get(&primary_thread, &pri_stats);
    k_thread_runtime_stats_get(&secondary_thread, &sec_stats);
    k_thread_runtime_stats_all_get(&end_stats);

    start = timing_cycles_to_ns(start_stats.execution_cycles) / 1000;
    primary = timing_cycles_to_ns(pri_stats.execution_cycles) / 1000;
    secondary = timing_cycles_to_ns(sec_stats.execution_cycles) / 1000;
    end = timing_cycles_to_ns(end_stats.execution_cycles) / 1000;
    elapsed = end - start;

    printk("primary %lld secondary %lld start %lld end %lld elapsed %lld (us)\n",
           primary, secondary, start, end, elapsed);

    *pri_duration = primary;
    *sec_duration = secondary;
    *total_duration = elapsed;

    k_thread_abort(&primary_thread);
    k_thread_abort(&secondary_thread);
}

void run_analyzer(k_thread_entry_t thread_entry,
                  void *arg,
                  int pri_prio,  k_timeout_t pri_delay,
                  uint64_t *pri_duration,
                  int sec_prio,  k_timeout_t sec_delay,
                  uint64_t *sec_duration,
                  uint64_t *total_duration)
{
    run_analyzer_split(thread_entry,
                       primary_name, arg, NULL,
                       pri_prio, pri_delay, pri_duration,
                       thread_entry,
                       secondary_name, arg, NULL,
                       sec_prio, sec_delay, sec_duration,
                       total_duration);

}

void test_priority_inversion(void)
{
    uint64_t high_stats, low_stats, elapsed_stats;
    struct k_sem sem;
    k_sem_init(&sem, 1, 1);

    run_analyzer((k_thread_entry_t)priority_inversion, &sem,
                 K_PRIO_PREEMPT(4), K_MSEC(10), &low_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, high_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, low_stats);
}

void produce_uart(struct device *uart, uint32_t *count)
{
    for(int i = 0; ; i++) {
        // Send the 95 printable ASCII characters twice
        for (char c = ' '; c < 0x7f; c++) {
            uart_poll_out(uart, c);
            (*count)++;
        }
        if (!(i % 2)) {
            k_sleep(K_MSEC(1));
        }
    }
}

void consume_uart(struct device *uart, uint32_t *count)
{
    char c;
    while (1) {
        while (!uart_poll_in(uart, &c)) {
            (*count)++;
        }
        k_sleep(K_MSEC(1));
    }
}

void test_overflow(void)
{
    uint64_t consume_stats, produce_stats, elapsed_stats;
    struct device *uart;
    uint32_t produce_count, consume_count;
    run_analyzer_split((k_thread_entry_t)produce_uart, uart, &produce_count, NULL,
                       K_PRIO_PREEMPT(4), K_MSEC(10), &produce_stats,
                       (k_thread_entry_t)consume_uart, uart, &consume_count, NULL,
                       K_PRIO_PREEMPT(3), K_MSEC(12), &consume_stats,
                       &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, consume_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, produce_stats);
    TEST_ASSERT_UINT32_WITHIN(1000, 0, produce_count);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, consume_count);
}


void test_coop__no_priority__no_yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &first_stats,
                 K_PRIO_COOP(3), K_MSEC(12), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, second_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, first_stats);
}

void test_preempt__no_priority__no_yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &first_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(12), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, second_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, first_stats);
}

void test_coop__no_priority__yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &first_stats,
                 K_PRIO_COOP(3), K_MSEC(10), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, first_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, second_stats);
}

void test_preempt__no_priority__yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &first_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, first_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, second_stats);
}

void test_coop__priority_low_first__no_yield(void)
{
    uint64_t high_stats, low_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, high_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, low_stats);
}

void test_coop__priority_high_first__no_yield(void)
{
    uint64_t high_stats, low_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(3), K_MSEC(12), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(10), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_coop__priority_low_first__yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_coop__priority_high_first__yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_preempt__priority__no_yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                 K_PRIO_PREEMPT(2), K_MSEC(10), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_preempt__priority__yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                 K_PRIO_PREEMPT(2), K_MSEC(10), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}


void test_mix__priority__yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(3), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}


void test_preempt__priority__sleepy(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer_split((k_thread_entry_t)busy_busy, NULL, NULL, NULL,
                       K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                       (k_thread_entry_t)busy_sleep, NULL, NULL, NULL,
                       K_PRIO_PREEMPT(2), K_MSEC(10), &high_stats,
                       &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 10000, high_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, low_stats);
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
    RUN_TEST(test_preempt__priority__sleepy);
    return UNITY_END();
}
