/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "pico/sync.h"
#include <pico/cyw43_arch.h>

static volatile bool fired = false;
datetime_t alarm = {
    .year  = -1,
    .month = -1,
    .day   = -1,
    .dotw  = -1,
    .hour  = -1,
    .min   = -1,
    .sec   = 0
};
int toggle = 0;
static void alarm_callback(void) {
    toggle = !toggle;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, toggle);
    printf("Fired!\n");
    alarm.sec = (alarm.sec + 1) % 60;
    rtc_set_alarm(&alarm, alarm_callback);
}

int main() {
    stdio_init_all();
    printf("RTC Alarm Repeat!\n");

    // Start on Wednesday 13th January 2021 11:20:00
    datetime_t t = {
        .year  = 2020,
        .month = 01,
        .day   = 13,
        .dotw  = 3, // 0 is Sunday, so 3 is Wednesday
        .hour  = 11,
        .min   = 20,
        .sec   = 50
    };

    hard_assert(cyw43_arch_init() == PICO_OK);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, toggle);
    // Start the RTC
    rtc_init();
    rtc_set_datetime(&t);
    rtc_set_alarm(&alarm, alarm_callback);


    // Alarm will keep firing forever
    while(1) __nop();
    return 0;
}
