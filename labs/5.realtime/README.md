# Lab 5
# Learning objectives
* Describe key concepts in time keeping.
* Measure the accuracy in a clock source.
* Measure the latency in a system operation.
* Measure the temporal operation of the OS kernel.
* Differentiate between global and local time.
* Recognize the complications of time syncronization between systems.
* Fear the year 2038.

# Pre-lab
## Readings
Read this section of the Zephyr documentation regarding time concepts.

https://docs.zephyrproject.org/2.7.5/reference/misc/timeutil.html#timeutil-concepts

Read this section of the Zephyr documentation regarding system time

https://docs.zephyrproject.org/2.7.5/reference/kernel/timing/timers.html

Review chapter 25 on the RTC available in the STM32. Compare with the general purpose timers in chapter 18.

https://www.st.com/resource/en/reference_manual/rm0091-stm32f0x1stm32f0x2stm32f0x8-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

Read this introduction to the year 2038 and be properly terrified.

https://theyear2038problem.com/

Read this introduction to real-time clocks.

https://www.digikey.com/en/articles/enabling-timekeeping-function-and-prolonging-battery-life-in-low-power-systems

Read this article about GPS time synchronization

https://safran-navigation-timing.com/guide-to-gps-gnss-clock-synchronization/

## Terms
Familiarize yourself with these terms from the readings or .

* Using time
    * Instant
    * Duration
    * Delay
    * Latency
    * Resolution
    * Monotonic
    * Continuous vs discrete
    * Jitter
    * Drift
    * Skew
    * Synchronization
* Types of clocks
    * Hardware Clock
    * Real-time clock
    * System time
* Global time
    * UTC
    * TAI
    * NTP
    * Unix time
* Operating system timer
    * Jiffy
    * Tick
    * Sleep
    * Kernel timer
    * Uptime
