#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>
#include <drivers/uart.h>
#include "helper.h"

void setUp(void) {}

void tearDown(void) {}

void produce_uart(struct device *uart, uint64_t *out_count, int64_t *worst_offset)
{
    for(int i = 0; ; i++) {
        // Send the 95 printable ASCII characters twice
        for (char c = ' '; c < 0x7f; c++) {
            uart_poll_out(uart, c);
            (*out_count)++;
        }
        for (char c = ' '; c < 0x7f; c++) {
            char r;
            uart_poll_in(uart, &r);
            int64_t offset = (int64_t)r - (int64_t)c;
            if (offset > *worst_offset || -offset > *worst_offset) {
                *worst_offset = offset;
            }
        }

        if (!(i % 8)) {
            k_sleep(K_MSEC(1));
        }
    }
}

void consume_uart(struct device *uart, uint64_t *count)
{
    char c;
    while (1) {
        while (!uart_poll_in(uart, &c)) {
            uart_poll_out(uart, c);
            (*count)++;
        }
        k_sleep(K_MSEC(1));
    }
}

#define UART2 DT_NODELABEL(usart2)
#define UART3 DT_NODELABEL(usart3)

void test_overflow_uart(const struct uart_config *config)
{
    uint64_t consume_stats, produce_stats, elapsed_stats;
    const struct device *uart2 = device_get_binding(DT_LABEL(UART2));
    const struct device *uart3 = device_get_binding(DT_LABEL(UART3));
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, uart_configure(uart2, config),
                                  "Unable to configure USART2");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, uart_configure(uart3, config),
                                  "Unable to configure USART3");

    uint64_t produce_count, consume_count;
    int64_t worst_offset;
    produce_count = consume_count = 0;
    worst_offset = 0;

    run_analyzer_split(100,
                       (k_thread_entry_t)produce_uart, uart2, &produce_count, &worst_offset,
                       K_PRIO_PREEMPT(4), K_MSEC(10), &produce_stats,
                       (k_thread_entry_t)consume_uart, uart3, &consume_count, NULL,
                       K_PRIO_PREEMPT(3), K_MSEC(12), &consume_stats,
                       &elapsed_stats);
    printk("produced %lld consumed %lld offset %lld", produce_count, consume_count, worst_offset);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, consume_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, produce_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, produce_count);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, consume_count);
}

void test_overflow_flow(void)
{
    const struct uart_config config = {
        9600,
        UART_CFG_PARITY_NONE,
        UART_CFG_STOP_BITS_1,
        UART_CFG_DATA_BITS_8,
        UART_CFG_FLOW_CTRL_RTS_CTS
    };
    test_overflow_uart(&config);
}

void test_overflow_noflow(void)
{
    const struct uart_config config = {
        9600,
        UART_CFG_PARITY_NONE,
        UART_CFG_STOP_BITS_1,
        UART_CFG_DATA_BITS_8,
        UART_CFG_FLOW_CTRL_NONE,
    };
    test_overflow_uart(&config);
}

int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_overflow_flow);
    RUN_TEST(test_overflow_noflow);
    return UNITY_END();
}
