#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/counter.h>
#include <stm32_ll_rtc.h>
#define RTC_DEV DT_NODELABEL(rtc)
#define TIMER DT_NODELABEL(timers14)

#if !DT_NODE_HAS_STATUS(RTC_DEV, okay)
#error "missing counter"
#endif

#if !DT_NODE_HAS_STATUS(TIMER, okay)
#error "missing counter"
#endif

#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0

const struct device *dev_gpio;
const struct device *dev_count;

struct counter_alarm_cfg alarm;

static void alarm_interrupt(const struct device *d,
                     uint8_t chan_id,
                     uint32_t ticks,
                     void *user_data)
{
    gpio_pin_toggle(dev_gpio, PIN_OUT);
	int err = counter_set_channel_alarm(dev_count, 0, user_data);
	if (err != 0) {
		printk("Alarm could not be set\n");
	}
}

void rtc_main(void)
{
	dev_gpio = device_get_binding(DT_LABEL(DEV_OUT));
    dev_count = device_get_binding(DT_LABEL(RTC_DEV));

	gpio_pin_configure(dev_gpio, PIN_OUT, GPIO_OUTPUT_ACTIVE);

    printk("Got binding\n");
	int err;

	if (dev_count == NULL) {
		printk("Device not found\n");
		return;
	}

    const struct counter_config_info *config =
			(const struct counter_config_info *)dev_count->config;
    int async_pre = 0x07;
    int sync_pre = 0x0007;
    int usec = 1000;


    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_EnableInitMode(RTC);
    while (!LL_RTC_IsActiveFlag_INIT(RTC));
    LL_RTC_SetAsynchPrescaler(RTC, async_pre);
    LL_RTC_SetSynchPrescaler(RTC, sync_pre);
    LL_RTC_DisableInitMode(RTC);
    LL_RTC_EnableWriteProtection(RTC);

	counter_start(dev_count);
	alarm.flags = 0;
    int real_freq = 40000 / (async_pre + 1) / (sync_pre + 1);
    int ticks = (counter_us_to_ticks(dev_count, usec) / config->freq) * real_freq;
	alarm.ticks = ticks;
	alarm.callback = alarm_interrupt;
	alarm.user_data = &alarm;
	err = counter_set_channel_alarm(dev_count, 0, &alarm);

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
    k_sleep(K_FOREVER);
}
