#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>
#include "helper.h"

void setUp(void) {}

void tearDown(void) {}

void produce_queue(struct k_msgq *queue, uint64_t *out_count, uint64_t *lost_count)
{
    printk("Starting produce\n");
    while(1) {
        // Send the 95 printable ASCII characters
        for (char c = ' '; c < 0x7f; c++) {
            uint32_t u = (uint32_t)c;
            (*out_count)++;
            if (k_msgq_put(queue, &u, K_NO_WAIT)) {
                (*lost_count)++;
            }
        }
    }
}

void produce_queue_polite(struct k_msgq *queue, uint64_t *out_count, uint64_t *lost_count)
{
    printk("Starting polite\n");
    while(1) {
        // Send the 95 printable ASCII characters as fast as possible
        for (char c = ' '; c < 0x7f; c++) {
            (*out_count)++;
            uint32_t u = (uint32_t)c;
            while (k_msgq_put(queue, &u, K_MSEC(1))) {
                (*lost_count)++;
            }
        }
    }
}

void consume_queue(struct k_msgq *queue, uint64_t *count)
{
    printk("Starting consume\n");
    uint32_t c;
    while (1) {
        k_msgq_get(queue, &c, K_FOREVER);
        k_busy_wait(1000);
        (*count)++;
    }
}

K_MSGQ_DEFINE(queue, sizeof(uint32_t), 1000, 4);

void test_overflow_queue(void)
{
    k_msgq_purge(&queue);
    uint64_t consume_stats, produce_stats, elapsed_stats;
    uint64_t produce_count, lost_count, consume_count;
    produce_count = lost_count = consume_count = 0;
    run_analyzer_split(100,
                       (k_thread_entry_t)produce_queue, &queue, &produce_count, &lost_count,
                       K_PRIO_PREEMPT(3), K_MSEC(10), &produce_stats,
                       (k_thread_entry_t)consume_queue, &queue, &consume_count, NULL,
                       K_PRIO_PREEMPT(2), K_MSEC(12), &consume_stats,
                       &elapsed_stats);
    printk("produced %lld consumed %lld lost %lld", produce_count, consume_count, lost_count);
    TEST_ASSERT_UINT64_WITHIN(200, 1500, produce_count);
    TEST_ASSERT_UINT64_WITHIN(20, 100, consume_count);
    TEST_ASSERT_UINT64_WITHIN(100, 500, lost_count);
}

void test_overflow_polite(void)
{
    k_msgq_purge(&queue);
    uint64_t consume_stats, produce_stats, elapsed_stats;
    uint64_t produce_count, lost_count, consume_count;
    produce_count = lost_count = consume_count = 0;
    run_analyzer_split(100,
                       (k_thread_entry_t)produce_queue_polite, &queue, &produce_count, &lost_count,
                       K_PRIO_PREEMPT(4), K_MSEC(10), &produce_stats,
                       (k_thread_entry_t)consume_queue, &queue, &consume_count, NULL,
                       K_PRIO_PREEMPT(3), K_MSEC(12), &consume_stats,
                       &elapsed_stats);
    printk("produced %lld consumed %lld lost %lld", produce_count, consume_count, lost_count);
    TEST_ASSERT_UINT64_WITHIN(200, 1000, produce_count);
    TEST_ASSERT_UINT64_WITHIN(20, 100, consume_count);
    TEST_ASSERT_UINT64_WITHIN(10, 0, lost_count);
}

int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_overflow_queue);
    RUN_TEST(test_overflow_polite);
    return UNITY_END();
}
