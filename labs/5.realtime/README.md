g# Lab 5
# Learning objectives
* Describe key concepts in time keeping.
* Measure the accuracy in a clock source.
* Measure the latency in a system operation.
* Measure the temporal operation of the OS kernel.
* Differentiate between global and local time.
* Recognize the complications of time syncronization between systems.
* Fear the year 2038.
* Create temporal accuracy and precision requirements for a system.

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

Read this section of the docs to get a feel for how the OS switches contexts.

https://docs.zephyrproject.org/2.7.5/guides/arch/arm_cortex_m.html#os-features

https://docs.zephyrproject.org/2.7.5/guides/porting/arch.html?highlight=systick#thread-context-switching

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
    * Slew
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

# Lab
## Activity
Copy in the source code for each of the files in the directory.

The files are:
* `sleep.c` : a loop with a delay using `k_sleep`.
* `kernel.c`: a loop with a delay using `k_timer`
* `rtc.c`: an alarm interrupt from the RTC

For each of the cases, use an oscilloscope to measure jitter and drift of the time keeping method.

Capture the data. They'll need some matlab or python to process the data.

Commit a report and the data.


## Activity
Measure the latency of an interrupt handler.

Run the code `gpio_interrupt.c`

When an interrupt is triggered on a GPIO pin, toggles another GPIO pin, goes back to sleep.

CPU is idle in `k_sleep(K_FOREVER)`;
Drive with a signal generator.

## Activity
Modify the latency by adding in a message queue. From the interrupt handler send a message.

Create a new thread that reads messages from the queue and toggles the output pin when a message is received.

Measure the latency again.
