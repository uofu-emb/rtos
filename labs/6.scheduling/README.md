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

### Setup
You'll need to add these config files.

zephyr/app.overlay
```
&usart2 {
    status = "okay";
    pinctrl-0 = < &usart2_tx_pa2 &usart2_rx_pa3 &usart2_cts_pa0 &usart2_rts_pa1 >;
    current-speed = < 0x1c200 >;
};

/ {
    soc {
        usart3: serial@40004800 {
            compatible = "st,stm32-usart", "st,stm32-uart";
            reg = <0x40004800 0x400>;
            clocks = <&rcc STM32_CLOCK_BUS_APB1 0x00040000>;
            interrupts = <29 0>;
            status = "okay";
            label = "UART_3";
            pinctrl-0 = < &usart3_cts_pb13 &usart3_rts_pb14 &usart3_tx_pb10 &usart3_rx_pb11 >;
			current-speed = < 0x1c200 >;
        };
    };
};
```

zephyr/prj.conf
```
CONFIG_SERIAL=y
CONFIG_NEWLIB_LIBC=y
CONFIG_NUM_METAIRQ_PRIORITIES=1
CONFIG_THREAD_RUNTIME_STATS=y
CONFIG_THREAD_RUNTIME_STATS_USE_TIMING_FUNCTIONS=y
```

When you create supervisor threads, you will need to set the priority to a Meta-IRQ level, which forces the thread to always preempt others. The priority should be set to `-CONFIG_NUM_COOP_PRIORITIES`.

https://docs.zephyrproject.org/2.7.5/reference/kernel/threads/index.html#meta-irq-priorities

To measure the runtime of threads, the runtime statistics can be gathered with timing functions. Of particular interest are `k_thread_runtime_stats_all_get` and `k_thread_runtime_stats_get`. Other timing functions are available that may be useful.

https://docs.zephyrproject.org/2.7.5/reference/kernel/threads/index.html#runtime-statistics

https://docs.zephyrproject.org/2.7.5/reference/timing_functions/index.html

When dealing with indeterminate data in tests due to variations in runtime, you may need to test bounded ranges.

https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#test_assert_int_within-delta-expected-actual

Systick is currently set a 1000Hz.

https://github.com/zephyrproject-rtos/zephyr/blob/v2.7-branch/drivers/clock_control/clock_stm32_ll_h7.c#L759

### Activity 0
Induce priority inversion.

1. create two preemptable threads (and a supervisor to manage them).
1. create a semaphore shared between the two.
1. set one to higher priority. set it to delay start.
1. have the lower priority thread acquire the semphore first.
1. verify that the high priority thread will not execute.

You may find it useful to create a helper function to setup and teardown the threads.

### Activity 1
In this activity, you'll create two competing threads that use the following functions
```
void busy_busy(char *name)
{
    for (int i = 0; ; i++);
}

void busy_yield(char *name)
{
    for (int i = 0; ; i++) {
        if (!(i & 0xFF)) {
            k_yield();
        }
    }
}
```
Write tests for two threads running the following scenarios. Try to predict the runtime for each thread.
1. Threads with same priority:
    1. Coop
        1. Both run `busy_busy`.
        1. Both run `busy_yield`
    1. Preempt
        1. Both run `busy_busy`.
        1. Both run `busy_yield`
1. Threads with different priority.
    1. Coop
        1. Both run `busy_busy`.
            1. Higher priority starts first.
            1. Lower priority starts first.
        1. Both run `busy_yield`
            1. Higher priority starts first.
            1. Lower priority starts first.
    1. Preempt
        1. Both run `busy_busy`.
            1. Higher priority starts first.
            1. Lower priority starts first.
        1. Both run `busy_yield`
            1. Higher priority starts first.
            1. Lower priority starts first.

Make sure you are setting priorities according to the priority order presented in the documentation.

### Activity 2

```
void busy_sleep(char *name)
{
    k_busy_wait(10000);
    k_sleep(K_MSEC(490));
}
```
1. Write tests for two threads: one should run `busy_sleep` and the other `busy_yield`
1. Predict the share of the runtime for each of the threads.

### Activity 3

The RS-232 communication protocol works by sending bytes at specific frequency called the _baudrate_. Data cannot be transmitted faster than the communication channel allows.

1. Create a producing thread that sends data using `uart_poll_out`
    1. It should repeatedly write out the 95 ASCII printable characters, in the range 0x20 to 0x7d inclusive.
    1. It should track a count of how many bytes it sent.
1. Create a consuming thread that reads data using `uart_poll_in`
    1. It should operate at a higher priority than the producer.
    1. It should read as many bytes are available.
    1. If no bytes are available, the thread should sleep for 1 ms.
    1. It should track how a count of how many bytes it received.
1. Connect the TX line to the RX line of the UART on your board.
1. Test the throughput of the system.
1. Verify that no data is being dropped.

### Activity 4
RS-232 includes additional pins that are used to coordinate and control between the data terminal equipment (DTE) and the data communication equipment (DCE). This historically was used by a dumb terminal or a computer (the DTE) connecting to a modem (the DCE).

The standard includes the request to send (RTS) and clear to send (CTS) signals. These can be used for _flow control_. If one of the devices cannot handle incoming data, it signals to the other device to halt transmission. THe difference between RTS and CTS lies in the direction of the connection. For our purposes, we will setup the UART in a _null modem_ configuration, with the two signals tied together.

https://docs.zephyrproject.org/2.7.5/reference/peripherals/uart.html#c.uart_config_flow_control.UART_CFG_FLOW_CTRL_RTS_CTS

1. Use the same producer/consumer pair from activity 3.
1. Connect the RTS and CTS pins on the board together.
1. Enable flow control.
1. Use the oscilloscope to observe the CTS line.
1. Measure the throughput again.
1. Verify that no data is being dropped.
