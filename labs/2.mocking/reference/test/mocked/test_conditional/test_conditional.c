#include <stdio.h>
#include <unity.h>
#include "uart_cond.h"

char *test_data = "hello world!\n";
char *current;

void test_uart_in(char *c)
{
    *c = *current;

    if (*current == '\n')
        current = test_data;
    else
        current++;
}

char *expected = "HELLO WORLD!\n";
char *current_expected;
void test_uart_out(const char c)
{
    TEST_ASSERT_EQUAL_CHAR(*current_expected++, c);
}

void setUp(void)
{
    current = test_data;
    current_expected = expected;
}

void tearDown(void) {}

void test_echo_upper(void)
{
    echo_uppercase_conditional((struct device *)0);
}

int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_echo_upper);
    return UNITY_END();
}
