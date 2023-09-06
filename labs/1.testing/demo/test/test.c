#include <unity.h>
#include "rtos.h"
#include "lab1.h"

int last_delayed;
void setUp(void) {
  last_delayed = 0;
}

void tearDown(void) {}

void delay_ms(int ms)
{
  last_delayed = ms;
}

void start_thread(void (* fun)(void))
{
}

void test_toggle_thread_setup()
{
  struct gpio dev = {0, 0};
  unsigned int led, counter;
  led = toggle_thread_setup(&dev, &counter);

  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, led,"LED did not start off.");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, last_delayed, "Delay was not called during setup.");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, counter, "Counter was not initialized");
  TEST_ASSERT_TRUE_MESSAGE(dev.CR & 1<<2, "Configuration bit was not set");
}

void test_toggle_thread_iteration_active()
{
  struct gpio dev = {0, 1 << 2};
  unsigned int led;
  unsigned int counter = 42;
  led = toggle_thread_iteration(1, &dev, &counter);

  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, led, "LED was not off.");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(500, last_delayed, "Delay was not called with correct value.");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(43, counter, "Counter was not incremented");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, dev.DR, "GPIO reg cleared");
}

void test_toggle_thread_iteration_inactive()
{
  struct gpio dev = {0, 0};
  unsigned int led;
  unsigned int counter = 0;
  led = toggle_thread_iteration(0, &dev, &counter);

  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, led,"LED was not on.");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(500, last_delayed, "Delay was not called with correct value.");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, counter, "Counter was not incremented");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, dev.CR, "Configuration bit was changed");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1 << 2, dev.DR, "GPIO reg st");
}

int main (void)
{
  UNITY_BEGIN();
  RUN_TEST(test_toggle_thread_setup);
  RUN_TEST(test_toggle_thread_iteration_inactive);
  RUN_TEST(test_toggle_thread_iteration_active);
  return UNITY_END();
}
