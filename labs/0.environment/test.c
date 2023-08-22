#include <stdint.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void testWorks()
{
    TEST_ASSERT_TRUE_MESSAGE(1 == 1,"Data bits not set as expected");
}

int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(testWorks);
    return UNITY_END();
}
