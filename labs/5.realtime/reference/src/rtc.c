#include <zephyr.h>
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/counter.h>

#define RTC DT_NODELABEL(rtc)
#define DEV_OUT DT_NODELABEL(gpioa)
#define PIN_OUT 0

const struct device *dev_out, *dev_count;

struct counter_alarm_cfg alarm;

void alarm_interrupt(const struct device *dev,
                     uint8_t chan_id,
                     uint32_t ticks,
                     void *user_data)
{
    gpio_pin_toggle(dev_out, PIN_OUT);
    counter_set_channel_alarm(dev_count, 1, &alarm);
}

void rtc_main(void)
{
    dev_out = device_get_binding(DT_LABEL(DEV_OUT));
    gpio_pin_configure(dev_out, PIN_OUT, GPIO_OUTPUT_ACTIVE);

    alarm.callback = alarm_interrupt;
    alarm.ticks = counter_us_to_ticks(dev_count, 1000);

    dev_count = device_get_binding(DT_LABEL(RTC));
    counter_set_channel_alarm(dev_count, 1, &alarm);
    counter_start(dev_count);
    k_sleep(K_FOREVER);
}
