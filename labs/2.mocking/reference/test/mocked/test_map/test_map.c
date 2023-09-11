#include <stdint.h>
#include <unity.h>
#include "map.h"

void setUp(void) {}

void tearDown(void) {}

int add5(int a)
{
    return a + 5;
}

void test_map(void)
{
    int array[10] = {0, 1, 2, 3, 4, 3, 2, 1, 0, 8};
    map(&array[0], 10, add5);
    int expected[10] = {5, 6, 7, 8, 9, 8, 7, 6, 5, 13};

    TEST_ASSERT_EQUAL_INT_ARRAY(array, expected, 10);
}

int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_map);
    return UNITY_END();
}
