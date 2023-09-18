#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>
#include <unity.h>
#include "loop.h"

void setUp(void) {}

void tearDown(void) {}

/**************** Activity 0-2 ****************/
void test_loop_blocks(void)
{
    struct k_sem semaphore;
    struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    k_sem_init(&semaphore, 1, 1);
    int counter = 0;

    k_sem_take(&semaphore, K_FOREVER);
    int result = do_loop(&timer, &semaphore, &counter, "test", K_MSEC(100));

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(0, counter);
}

void test_loop_runs(void)
{
    struct k_sem semaphore;
    struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    k_sem_init(&semaphore, 1, 1);
    int counter = 0;

    int result = do_loop(&timer, &semaphore, &counter, "test", K_MSEC(100));

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, counter);
}

/**************** Activity 3 ****************/
void test_fib_calc(void)
{
    TEST_ASSERT_EQUAL_INT(0, nth_fibonacci(0));
    TEST_ASSERT_EQUAL_INT(1, nth_fibonacci(1));
    TEST_ASSERT_EQUAL_INT(1, nth_fibonacci(2));
    TEST_ASSERT_EQUAL_INT(2, nth_fibonacci(3));
    TEST_ASSERT_EQUAL_INT(3, nth_fibonacci(4));
    TEST_ASSERT_EQUAL_INT(5, nth_fibonacci(5));
    TEST_ASSERT_EQUAL_INT(8, nth_fibonacci(6));
    TEST_ASSERT_EQUAL_INT(267914296, nth_fibonacci(42));
}

#define STACK_SIZE 100
#define THREAD_COUNT 4
K_THREAD_STACK_ARRAY_DEFINE(fib_stack, THREAD_COUNT, STACK_SIZE);
K_THREAD_STACK_DEFINE(sup_stack, STACK_SIZE);

void test_fibonacci(void)
{
    struct k_thread threads[THREAD_COUNT];
    struct k_thread *pool_threads[THREAD_COUNT];
    struct k_thread supervisor;
    struct thread_pool pool = {pool_threads, fib_stack, THREAD_COUNT, STACK_SIZE};

    int results[THREAD_COUNT];
    int args[THREAD_COUNT];
    int expected[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) {
        args[i] = i;
        expected[i] = nth_fibonacci(i);
        pool_threads[i] = &threads[i];
    }
    struct fibonacci calc = {args, results, THREAD_COUNT};

    int result = async_fibonacci(&supervisor, sup_stack, STACK_SIZE, &pool, &calc);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, results, THREAD_COUNT);
}

/**************** Activity 4 ****************/
K_THREAD_STACK_DEFINE(left_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(right_stack, STACK_SIZE);
struct k_thread left_thread, right_thread;

void test_deadlock(void)
{
    struct k_sem left, right;
    k_sem_init(&left, 1, 1);
    // Right starts locked,
    k_sem_init(&right, 0, 1);
    int counter;
    // Give threads semaphores in the opposite order
    k_thread_create(&left_thread, left_stack, STACK_SIZE,
                    (k_thread_entry_t) deadlock,
                    &left, &right, &counter,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);
    k_thread_create(&right_thread, right_stack, STACK_SIZE,
                    (k_thread_entry_t) deadlock,
                    &right, &left, &counter,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);
    k_busy_wait(50);
    k_thread_abort(&right_thread);
    k_thread_abort(&left_thread);
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&left));
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&right));
    TEST_ASSERT_EQUAL_INT(0, counter);
}


/**************** Activity 5 ****************/
void test_orphaned(void)
{
    int counter = 1;
    struct k_sem semaphore;
    k_sem_init(&semaphore, 1, 1);

    orphaned_lock(&semaphore, K_MSEC(500),&counter);
    TEST_ASSERT_EQUAL_INT(2, counter);
    // Note that reading and using the value of the semaphore
    // isn't usually a good idea in real situations.
    TEST_ASSERT_EQUAL_INT(1, k_sem_count_get(&semaphore));

    orphaned_lock(&semaphore, K_MSEC(500), &counter);
    TEST_ASSERT_EQUAL_INT(3, counter);
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&semaphore));

    orphaned_lock(&semaphore, K_MSEC(500), &counter);
    TEST_ASSERT_EQUAL_INT(3, counter);
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&semaphore));
}

void test_unorphaned(void)
{
    int counter = 1;
    struct k_sem semaphore;
    k_sem_init(&semaphore, 1, 1);

    int result;
    result = unorphaned_lock(&semaphore, K_MSEC(500), &counter);
    TEST_ASSERT_EQUAL_INT(2, counter);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, k_sem_count_get(&semaphore));

    result = unorphaned_lock(&semaphore, K_MSEC(500), &counter);
    TEST_ASSERT_EQUAL_INT(3, counter);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, k_sem_count_get(&semaphore));
}

/**************** runner ****************/
int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_loop_blocks);
    RUN_TEST(test_loop_runs);
    RUN_TEST(test_orphaned);
    RUN_TEST(test_unorphaned);
    RUN_TEST(test_fib_calc);
    RUN_TEST(test_fibonacci);
    RUN_TEST(test_deadlock);
    return UNITY_END();
}
