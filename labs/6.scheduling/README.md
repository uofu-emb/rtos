# Lab 6. Scheduling

## Learning objectives.
Evaluate production/consumption ratio to prevent overflow.
Classify timing requirements as hard, firm, or soft real-time requirements
Prioritize tasks based on timing requirements.
Identify cases of priority inversion.
Identify the difference between cooperative and preemptive prioritization.
Identify reschedule points in the operating system.
Analyze and select scheduling algorithms based on their ability to meet real-time requirements
Build systems that meet hard timing requirements.

## Pre-lab
Watch this video about producer/consumer throughput.

https://www.youtube.com/watch?v=AnHiAWlrYQc&ab_channel=ParamountPlus

Read sections 6.1 and 6.3 of this chapter.

https://www.sciencedirect.com/science/article/pii/B9780123749574000153#s0145

Read this section of the documentation

https://docs.zephyrproject.org/2.7.5/reference/kernel/scheduling/index.html

### Questions
What are reschedule points?

What is the difference between a preemptive and cooperative slicing?

Give an example of a hard real-time requirement. What are the consequences of missing the deadline?

Give an example of a soft real-time requirement. What are the consequences of missing the deadline?

How could you use cooperative threads to implement mutually exclusion without the need for a kernel object, such as a semaphore.

## Lab
https://docs.zephyrproject.org/2.7.5/reference/kconfig/CONFIG_THREAD_RUNTIME_STATS.html#std-kconfig-CONFIG_THREAD_RUNTIME_STATS

Systick is currently set a 1000Hz.
https://github.com/zephyrproject-rtos/zephyr/blob/v2.7-branch/drivers/clock_control/clock_stm32_ll_h7.c#L759

### Activity 0
Some kind of producer that overflows a consumer.
Maybe a rs232 uart. then they can use another board to drive it?
use cts/rts?
https://docs.zephyrproject.org/2.7.5/reference/peripherals/uart.html#c.uart_config_flow_control.UART_CFG_FLOW_CTRL_RTS_CTS

### Activity 1
Induce priority inversion.

1. create two preemptable threads.
1. create a semaphore shared between the two.
1. set one to higher priority. set it to delay start.
1. have the lower priority thread acquire the semphore.
1. verify that the high priority thread will not execute.

### Activity 2
Spin up two threads, with busy wait counters
https://docs.zephyrproject.org/2.7.5/guides/debug_tools/thread-analyzer.html
Set the scheduler to different types.
- preemptive
- cooperative

compare values after some amount of time.

Cases:
same priority:
coop, no yield, run forever, should see the second starve.
preempt, no yield, run forever, should see even use.

coop, yield, should see even use
preempt, yield, should see even

diff priority
coop higher priority starts, no yield, should starve lower.
coop lower priority starts, no yield, should starve higher.
coop, higher priority starts, yields, should starve lower.
preempt, higher priority starts, no yield, should starve lower.
preempt, lower priority starts, no yield, should starve lower.
preempt, higher priority starts, yields, should starve lower.
