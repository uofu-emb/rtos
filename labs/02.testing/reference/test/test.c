#include <stdint.h>
#include <stdio.h>
#include <unity.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include "blink.h"

#define LED_NODE DT_ALIAS(led3)
#define LED	DT_GPIO_LABEL(LED_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED_NODE, gpios)

void setUp(void) {}

void tearDown(void) {}

void test_main_thread_enable(void)
{
	const struct device *dev = device_get_binding(LED);

	int led_is_on = main_thread_setup(dev, PIN, FLAGS);
    TEST_ASSERT_TRUE_MESSAGE(!led_is_on, "LED flag should start toggled off");

    led_is_on = main_thread_iteration(dev, PIN, led_is_on);
    TEST_ASSERT_TRUE_MESSAGE(led_is_on, "LED flag should be toggled on");
    TEST_ASSERT_TRUE_MESSAGE(gpio_pin_get(dev, PIN) , "GPIO should be toggled on");

}

int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_main_thread_enable);
    return UNITY_END();
}
