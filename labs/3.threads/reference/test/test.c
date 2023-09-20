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
#define STACK_SIZE 200
K_THREAD_STACK_DEFINE(left_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(right_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(sup_stack, STACK_SIZE);
struct k_thread left_thread, right_thread, sup_thread;
struct k_sem left, right;

int counter1 = 0;
int counter0 = 42;
void deadlock_supervisor(void)
{
    k_sem_init(&left, 1, 1);
    // Right starts locked,
    k_sem_init(&right, 1, 1);
    printf("- Creating threads\n");
    printf("- address deadlock %d\n", (int)deadlock);
    k_tid_t l = k_thread_create(&left_thread, left_stack, STACK_SIZE,
                                (k_thread_entry_t) deadlock,
                                &left, &right, &counter1,
                                K_PRIO_COOP(6),
                                0,
                                K_NO_WAIT);
    k_tid_t r = k_thread_create(&right_thread, right_stack, STACK_SIZE,
                                (k_thread_entry_t) deadlock,
                                &left, &right, &counter0,
                                K_PRIO_COOP(6),
                                0,
                    K_NO_WAIT);
    printf("-Created threads\n");
    printf("-Waiting\n");
    for (int i = 0; i < 10; i++) {
        printf("Status left %s\n", k_thread_state_str(l));
        printf("Status right %s\n", k_thread_state_str(r));
        k_sleep(K_MSEC(10));
    }
    printf("-Killing threads\n");
    k_thread_abort(l);
    k_thread_abort(r);
    printf("-Killed threads\n");
}

void test_deadlock(void)
{
    printf("-Starting deadlock test\n");
    k_thread_create(&sup_thread, sup_stack, STACK_SIZE,
                    (k_thread_entry_t) deadlock_supervisor,
                    NULL, NULL, NULL,
                    K_PRIO_COOP(6),
                    0,
                    K_NO_WAIT);
    k_thread_join(&sup_thread, K_FOREVER);
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&left));
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&right));
    TEST_ASSERT_EQUAL_INT(44, counter0);
    TEST_ASSERT_EQUAL_INT(2, counter1);
}

/**************** Activity 4 ****************/
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
    RUN_TEST(test_deadlock);
    RUN_TEST(test_loop_blocks);
    RUN_TEST(test_loop_runs);
    RUN_TEST(test_orphaned);
    RUN_TEST(test_unorphaned);
    return UNITY_END();
}
