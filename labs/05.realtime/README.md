# Lab 5
# Learning objectives
* Describe key concepts in time keeping.
* Measure the accuracy in a clock source.
* Measure the latency in a system operation.
* Measure the temporal operation of the OS kernel.
* Differentiate between global and local time.
* Recognize the complications of time syncronization between systems.
* Fear the year 2038.
* Create temporal accuracy and precision requirements for a system.
* Practice your oscilloscope skills.

# Pre-lab
## Readings
Read this section of the Zephyr documentation regarding time concepts (you can ignore any references to Zephyr APIs).

https://docs.zephyrproject.org/2.7.5/reference/misc/timeutil.html#timeutil-concepts


Review chapter 4.8 on the RTC in the [Pico datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf). Compare with the general purpose timers in chapter 4.6..

Read this introduction to the year 2038 and be properly terrified.

https://theyear2038problem.com/

Read this introduction to real-time clocks.

https://www.digikey.com/en/articles/enabling-timekeeping-function-and-prolonging-battery-life-in-low-power-systems

Read this section of the docs to get a feel for how the OS switches contexts.

https://www.freertos.org/Documentation/02-Kernel/05-RTOS-implementation-tutorial/02-Building-blocks/03-The-RTOS-tick

https://www.freertos.org/Documentation/02-Kernel/05-RTOS-implementation-tutorial/02-Building-blocks/09-Saving-the-RTOS-task-context

https://www.freertos.org/Documentation/02-Kernel/05-RTOS-implementation-tutorial/02-Building-blocks/10-Restoring-the-context

## Terms
Familiarize yourself with these terms from the readings.

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
* `task_delay.c` : a loop with a delay using a thread sleep.
* `timer.c`: an timer interrupt.
* `sleep.c`: a loop with a sleep delay.

1. For each of the cases, use an oscilloscope to measure jitter and drift of the time keeping method.
   1. Read through the code, see what it does.
   1. Load the code onto your board
   1. Measure the accuracy of the timekeeping (see instructions below)
   1. Once you are finished, modify the code - introduce a busy wait loop after the GPIO is toggled.
       1. Busy wait sets the processor in a tight loop (usually a counter loop with a noop body).
       1. Measure the accuracy. Make sure to block execute for a long enough time, such as would come from reading 1KB of data or doing some nasty computation.
1. Measure the jitter of the Agilent function generator operating at the same frequency for comparison.

### Measuring jitter with the R&S RTM3004
More information in the manual section 8.2

1. Set source to the appropriate channel
1. Press the "Meas" button.
1. Select "Meas. Place" and select 1 to add a first measurement.
1. Set "Type" to "Period"
1. Set "Statistics" to on (should be blue)
1. Set "Measure X" to on if it isn't.
1. Statistics for current, min, max, mean, stddev, and count should be shown.
1. Select "Meas. Place" and select 2 to add a second measurement.
1. Add another measurement for frequency.
1. Add another measurement for duty cycle.
1. Using these measurements, calculate the drift over a 1 hour period.

### Capturing data with the RTM3004
More information in the manual section 10.2 for flash drive and section 12.2 for MTP. (I wrote these instructions when I was learning the features of the scope, you don't need to do this data capture during the lab.)

1. To save to a flash drive inserted into the front panel
    1. You can save the complete capture history, though this can be a lot of data. It's easier to capture the current display data.
        1. Set a large display by setting the horizontal scale to 100s
        1. Wait a while.
    1. Stop acquisition with "Run Stop" in the Trigger section
    1. Press "Save Load" in the Action section.
    1. Select "Waveforms" from the touchscreen.
    1. Change "Points" to "Display data"
    1. CSV is a good file format for use with other tools.
1. Transfer files with MTP.
    1. Connect the USB cable from the rear of the scope to your computer.
    1. If you are on a Windows or Linux machine, it should appear as an attached drive.
        1. Apple still refuses to implement MTP in OSX natively. I wasn't able to get OpenMTP working, your mileage may vary.
    1. Data should be "Live Data/Channel/Acquisition Memory"

Manual for the scope is here:

https://www.rohde-schwarz.com/us/manual/r-s-rtm3000-user-manual-manuals_78701-508356.html
## Activity
Measure the latency of an interrupt handler.

1. Review and run the code `gpio_interrupt.c`
1. Attach the Sync output from the signal generator to the oscilloscope and use as the trigger.
1. Drive the board with the signal generator output on pin A1.
   1. **Make sure to check the output voltages with the oscilloscope before attaching it to your board or you'll let the magic smoke out!** Set amplitude HiLevel to 3.3V, LoLevel to 0.0 V.
1. Measure the delay between the sync signal and the output from the board.
1. Increase the delay using a busy wait loop as before. (Put the delay before the toggle)

## Activity
**Skip this activity**
1. ~~Modify the interrupt handler by adding in a message queue.~~
1. ~~From the interrupt handler send a message.~~
1. ~~Create a new thread that reads messages from the queue and toggles the output pin when a message is received.~~
1. ~~Measure the latency again.~~
1. ~~Add a busy wait delay to the message handler. (Put the delay before the toggle)~~

# Reference implementation
A working reference implementation is available here https://github.com/uofu-emb/rtos.05
