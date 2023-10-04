#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/counter.h>

#define RTC DT_NODELABEL(rtc)
#define TIMER DT_NODELABEL(counters2)

#if !DT_NODE_HAS_STATUS(RTC, okay)
#error "missing counter"
#endif

#if !DT_NODE_HAS_STATUS(TIMER, okay)
#error "missing counter"
#endif

#define LED0_NODE DT_ALIAS(led0)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0	DT_GPIO_FLAGS(LED0_NODE, gpios)

#define LED1_NODE DT_ALIAS(led1)
#define LED1	DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1	DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1	DT_GPIO_FLAGS(LED1_NODE, gpios)

#define LED2_NODE DT_ALIAS(led2)
#define LED2	DT_GPIO_LABEL(LED2_NODE, gpios)
#define PIN2	DT_GPIO_PIN(LED2_NODE, gpios)
#define FLAGS2	DT_GPIO_FLAGS(LED2_NODE, gpios)

#define LED3_NODE DT_ALIAS(led3)
#define LED3	DT_GPIO_LABEL(LED3_NODE, gpios)
#define PIN3	DT_GPIO_PIN(LED3_NODE, gpios)
#define FLAGS3	DT_GPIO_FLAGS(LED3_NODE, gpios)

#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0

const struct device *dev;
const struct device *dev_gpio;
const struct device *dev_count;

struct counter_alarm_cfg alarm;

static void alarm_interrupt(const struct device *d,
                     uint8_t chan_id,
                     uint32_t ticks,
                     void *user_data)
{
    gpio_pin_toggle(dev, PIN2);
    gpio_pin_toggle(dev_gpio, PIN_OUT);

    struct counter_alarm_cfg *config = user_data;
    uint32_t now_ticks;
	uint64_t now_usec;
	int now_sec;
	int err;
	err = counter_get_value(dev_count, &now_ticks);
	if (err) {
		printk("Failed to read counter value (err %d)", err);
		return;
	}

	now_usec = counter_ticks_to_us(dev_count, now_ticks);
	now_sec = (int)(now_usec / USEC_PER_SEC);

	printk("!!! Alarm !!!\n");
	printk("Now: %u\n", now_sec);

	/* Set a new alarm with a double length duration */
	config->ticks = config->ticks * 2U;

	printk("Set alarm in %u sec (%u ticks)\n",
	       (uint32_t)(counter_ticks_to_us(dev_count,
					   config->ticks) / USEC_PER_SEC),
	       config->ticks);

	err = counter_set_channel_alarm(dev_count, 0,
					user_data);
	if (err != 0) {
		printk("Alarm could not be set\n");
	}
}

void rtc_main(void)
{
	dev = device_get_binding(LED1);
	dev_gpio = device_get_binding(DT_LABEL(DEV_OUT));
    dev_count = device_get_binding(DT_LABEL(RTC));

	int err;

	printk("Counter alarm sample\n\n");
	if (dev_count == NULL) {
		printk("Device not found\n");
		return;
	}

	counter_start(dev_count);

	alarm.flags = 0;
	alarm.ticks = counter_us_to_ticks(dev_count, 20000);
	alarm.callback = alarm_interrupt;
	alarm.user_data = &alarm;

	err = counter_set_channel_alarm(dev_count, 0, &alarm);
	printk("Set alarm in %u sec (%u ticks)\n",
	       (uint32_t)(counter_ticks_to_us(dev_count,
					   alarm.ticks) / USEC_PER_SEC),
	       alarm.ticks);

	if (-EINVAL == err) {
		printk("Alarm settings invalid\n");
        return;
	} else if (-ENOTSUP == err) {
		printk("Alarm setting request not supported\n");
        return;
	} else if (err != 0) {
		printk("Error\n");
        return;
	}

    gpio_pin_configure(dev, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS1);
    gpio_pin_configure(dev, PIN2, GPIO_OUTPUT_ACTIVE | FLAGS1);
	gpio_pin_configure(dev_gpio, PIN_OUT, GPIO_OUTPUT_ACTIVE);
    k_sleep(K_FOREVER);
}
