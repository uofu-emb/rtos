#include <stdio.h>
#include <zephyr.h>
#include <unity.h>
#include "fifo.h"

#define STACKSIZE 2000
#define THREAD_COUNT 4

int setup_pool;

struct k_thread supervisor_thread;
K_THREAD_STACK_DEFINE(supervisor_stack, STACKSIZE);

struct k_thread worker_threads[THREAD_COUNT];
K_THREAD_STACK_ARRAY_DEFINE(worker_stacks, THREAD_COUNT, STACKSIZE);

K_MSGQ_DEFINE(request, sizeof(struct request_msg), 32, 4);
K_MSGQ_DEFINE(response, sizeof(struct request_msg), 32, 4);

void setUp(void)
{
    if (setup_pool) {
        for (int t = 0; t < THREAD_COUNT; t++) {
            k_thread_create(&worker_threads[t],
                            worker_stacks[t],
                            STACKSIZE,
                            (k_thread_entry_t) fifo_worker_handler,
                            &request,
                            &response,
                            (void *) t,
                            K_PRIO_COOP(7),
                            0,
                            K_NO_WAIT);
        }
    }
}

void tearDown(void)
{
    k_msgq_purge(&request);
    k_msgq_purge(&response);
    if (setup_pool) {
        for (int t = 0; t < THREAD_COUNT; t++) {
            k_thread_abort(&worker_threads[t]);
        }
    }
}

void test_full(void)
{
    printf("Sending messages\n");
    for (int i = 0; i < 32; i++) {
        struct request_msg data = {};
        data.input = i;
        k_msgq_put(&request, &data, K_FOREVER);
    }

    for (int i = 0; i < 32; i++) {
        struct request_msg data = {};
        int res = k_msgq_get(&response, &data, K_MSEC(1000));
        TEST_ASSERT_EQUAL_INT(0, res);
        printf("Got result %d for %d, handled by thread %d\n",
               data.input, data.output, data.handled_by);
        TEST_ASSERT_EQUAL_INT(data.input + 5, data.output);
    }

    printf("Done\n");
}

void test_single(void)
{
    struct request_msg data = {42, -1, 0};
    k_msgq_put(&request, &data, K_FOREVER);
    k_msgq_get(&response, &data, K_FOREVER);
    TEST_ASSERT_EQUAL_INT(data.input, 42);
    TEST_ASSERT_EQUAL_INT(data.input + 5, data.output);

    TEST_ASSERT_EQUAL_INT(-EAGAIN, k_msgq_get(&response, &data, K_MSEC(1000)));
}

void test_nothing(void)
{
    struct request_msg data = {42, -1, 0};
    k_msgq_put(&request, &data, K_FOREVER);
    k_msgq_get(&response, &data, K_FOREVER);
    TEST_ASSERT_EQUAL_INT(data.input, 42);
    TEST_ASSERT_EQUAL_INT(data.input + 5, data.output);
    TEST_ASSERT_EQUAL_INT(-EAGAIN, k_msgq_get(&response, &data, K_MSEC(1000)));
}

void test_all_alone(void)
{
    // Note this test should run with no thread pool.
    struct request_msg data = {42, -1, 0};
    k_msgq_put(&request, &data, K_FOREVER);
    TEST_ASSERT_EQUAL_INT(-EAGAIN, k_msgq_get(&response, &data, K_MSEC(1000)));
}


int main (void)
{
    setup_pool = 1;
    UNITY_BEGIN();
    RUN_TEST(test_full);
    RUN_TEST(test_nothing);
    RUN_TEST(test_single);
    setup_pool = 0;
    RUN_TEST(test_all_alone);
    return UNITY_END();
}
