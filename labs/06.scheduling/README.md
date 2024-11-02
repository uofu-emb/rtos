# Lab 6. Scheduling

## Learning objectives.
* Evaluate production/consumption ratio to prevent overflow.
* Classify timing requirements as hard, firm, or soft real-time requirements
* Prioritize tasks based on timing requirements.
* Identify cases of priority inversion.
* Identify the difference between cooperative and preemptive prioritization.
* Identify reschedule points in the operating system.
* Analyze and select scheduling algorithms based on their ability to meet real-time requirements
* Build systems that meet hard timing requirements.

## Pre-lab
Watch this video about producer/consumer throughput.

https://www.youtube.com/watch?v=AnHiAWlrYQc&ab_channel=ParamountPlus

Review these sections of the documentation

https://www.freertos.org/Documentation/01-FreeRTOS-quick-start/01-Beginners-guide/01-RTOS-fundamentals

https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/03-Task-priorities

https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/04-Task-scheduling

Read chapter 12 in Lee & Seshia.

Read sections 6.1 and 6.3 of this chapter.

https://www.sciencedirect.com/science/article/pii/B9780123749574000153#s0145

# Lab

When you create supervisor threads, you will need to set the priority higher than subordinate threads, which forces the supervisor thread to always preempt others.

https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/03-Task-priorities

To measure the runtime of threads, the runtime statistics can be gathered with task utility functions. Of particular interest are `vTaskGetInfo` and `uxTaskGetSystemState`. Other timing functions are available that may be useful. Check to see if there is any configuration changes that need to be made to enable metric gathering.

https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/00-Task-utilities

When dealing with indeterminate data in tests due to variations in runtime, you may need to test bounded ranges.

https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md#test_assert_int_within-delta-expected-actual

### Activity 0
Induce priority inversion.

1. create two preemptable threads (and a supervisor to manage them).
1. create a semaphore shared between the two. Create it with xSemaphoreCreateBoolean
1. set one thread to higher priority. set it to delay start.
1. have the lower priority thread acquire the semphore first.
1. Predict the behavior of the system.

You may find it useful to create a helper function to setup and teardown the threads.

### Activity 1
Repeat the previous experiment, but this time create the semaphore with xSemaphoreCreateMutex

### Activity 2
In this activity, you'll create two competing threads that use the following functions
```
void busy_busy(void)
{
    for (int i = 0; ; i++);
}

void busy_yield(void)
{
    for (int i = 0; ; i++) {
        taskYIELD();
    }
}
```
Write tests for two threads running the following scenarios. Try to predict the runtime for each thread.
1. Threads with same priority:
    1. Both run `busy_busy`.
    1. Both run `busy_yield`
    1. One run `busy_busy` one run `busy_yield`
1. Threads with different priority.
    1. Both run `busy_busy`.
        1. Higher priority starts first.
        1. Lower priority starts first.
    1. Both run `busy_yield`.

Make sure you are setting priorities according to the priority order presented in the documentation.

# Reference implementation
Reference implementation is located at https://github.com/uofu-emb/rtos.06
