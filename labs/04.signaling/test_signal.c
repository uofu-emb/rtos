#include <stdio.h>
#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <unity.h>
#include "signaling.h"

#define TEST_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define TEST_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define TEST_RUNNER_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define TEST_RUNNER_STACK_SIZE configMINIMAL_STACK_SIZE

void setUp(void) {}

void tearDown(void) {}

struct task_args
{
    SemaphoreHandle_t request;
    SemaphoreHandle_t response;
    struct signal_data *data;
};

void calc_task(void *vargs)
{
    struct task_args *args = (struct task_args *)vargs;
    while(1) {
        signal_handle_calculation(args->request,
                                  args->response,
                                  args->data);
    }
    vTaskDelete(NULL);
}

void test_request(void)
{
    TaskHandle_t coop_thread;
    SemaphoreHandle_t request = xSemaphoreCreateCounting(1, 0);
    SemaphoreHandle_t response = xSemaphoreCreateCounting(1, 0);

    struct signal_data data = {};
    struct task_args args = {request, response, &data};
    xTaskCreate(calc_task, "test_request", TEST_TASK_STACK_SIZE,
                (void *)&args, TEST_TASK_PRIORITY, &coop_thread);
    for (int counter = 46; counter < 55; counter++) {
        data.input = counter;
        BaseType_t result = signal_request_calculate(request, response, &data);
        TEST_ASSERT_EQUAL_INT(pdTRUE, result);
        TEST_ASSERT_EQUAL_INT(counter+5, data.output);
	}
    vTaskDelete(coop_thread);
    vSemaphoreDelete(request);
    vSemaphoreDelete(response);
}

void test_noone_home(void)
{
    SemaphoreHandle_t response = xSemaphoreCreateCounting(1, 0);
    SemaphoreHandle_t request = xSemaphoreCreateCounting(1, 0);
    struct signal_data data = {42, 42};
    struct task_args args = {request, response, &data};
    BaseType_t result = signal_request_calculate(request, response, &data);
    TEST_ASSERT_EQUAL_INT(pdFALSE, result);
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(request));
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(response));
    TEST_ASSERT_EQUAL_INT(42, data.input);
    TEST_ASSERT_EQUAL_INT(42, data.output);
    vSemaphoreDelete(request);
    vSemaphoreDelete(response);
}

void test_noop(void)
{
    TaskHandle_t coop_thread;
    SemaphoreHandle_t request = xSemaphoreCreateCounting(1, 0);
    SemaphoreHandle_t response = xSemaphoreCreateCounting(1, 0);
    struct signal_data data = {42, 42};
    struct task_args args = {request, response, &data};
    xTaskCreate(calc_task, "test_noop", TEST_TASK_STACK_SIZE,
                (void *)&args, TEST_TASK_PRIORITY, &coop_thread);
    vTaskDelay(1000);
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(request));
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(response));
    TEST_ASSERT_EQUAL_INT(42, data.input);
    TEST_ASSERT_EQUAL_INT(42, data.output);
    vTaskDelete(coop_thread);
    vSemaphoreDelete(request);
    vSemaphoreDelete(response);
}


void test_out_of_order(void)
{
    TaskHandle_t coop_thread;
    SemaphoreHandle_t request = xSemaphoreCreateCounting(1, 0);
    SemaphoreHandle_t response = xSemaphoreCreateCounting(1, 0);
    struct signal_data data = {42, 42};
    struct task_args args = {request, response, &data};
    xTaskCreate(calc_task, "test_out_of_order", TEST_TASK_STACK_SIZE,
                (void *)&args, TEST_TASK_PRIORITY, &coop_thread);
    xSemaphoreGive(response);
    vTaskDelay(1000);
    TEST_ASSERT_EQUAL_INT(0, uxSemaphoreGetCount(request));
    TEST_ASSERT_EQUAL_INT(1, uxSemaphoreGetCount(response));
    TEST_ASSERT_EQUAL_INT(42, data.input);
    TEST_ASSERT_EQUAL_INT(42, data.output);
    vTaskDelete(coop_thread);
    vSemaphoreDelete(request);
    vSemaphoreDelete(response);
}

void runner_thread (__unused void *args)
{
    for (;;) {
        printf("Starting test run.\n");
        UNITY_BEGIN();
        RUN_TEST(test_noop);
        RUN_TEST(test_out_of_order);
        RUN_TEST(test_request);
        RUN_TEST(test_noone_home);
        UNITY_END();
        sleep_ms(5000);
    }
}

int main (void)
{
    stdio_init_all();
    printf("Launching runner\n");
    hard_assert(cyw43_arch_init() == PICO_OK);
    xTaskCreate(runner_thread, "TestRunner",
                TEST_RUNNER_STACK_SIZE, NULL, TEST_RUNNER_PRIORITY, NULL);
    vTaskStartScheduler();
	return 0;
}
