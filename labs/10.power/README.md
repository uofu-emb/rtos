# Lab 10. Power
In this lab we'll be exploring low power modes in the Raspberry 2040.

# Learning Objectives
* Read and apply datasheets
* Read electronic schematics
* Use benchtop power and measurement equipment
* Describe the sources of power consumption.
* Enable low power modes.
* Working with open-source library code.

# Prelab
Review this documentation, ignoring the manufacturer sales pitches.

https://www.fluke.com/en-us/learn/blog/electrical/what-is-current?srsltid=AfmBOorO5bu93JN_4_OivS3Mc83iEZTXmJFJVjT2jZN7OMxMaGqmxylX

https://www.keysight.com/used/us/en/knowledge/guides/how-to-measure-current-with-a-multimeter

https://www.keysight.com/used/us/en/knowledge/guides/how-to-measure-current-in-a-circuit

Read sections 2.9, 2.10, 2.11, 5.7, 5.6 and 5.7 in the RP2040 datasheet.

https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf

Read sections 3.4 and 3.5 in the Pico-W datasheet.

https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf

We'll be using a supplementary library from Raspberry Pi to provide functions to put the controller to sleep.
As you did with Unity and FreeRTOS, add a submodule and modify your CMakeLists.txt to include the pico_sleep library.
Instructions for CMake are in the README at the bottom of the repo.

https://github.com/raspberrypi/pico-extras

# Lab

**DO NOT CONNECT YOUR PICO TO BOTH AN EXTERNAL POWER SUPPLY AND USB!!!!**. We don't like magic smoke escaping from anyone's Pico, Laptop, or benchtop equipment.


## Activity
1. Identify the correct location on the Pico board to attach the external power supply.
1. Configure the benchtop power supply to an appropriate voltage.

Hint: Consult the schematics and datasheets you read in the prelab.

## Activity
1. Determine what the default values for the `WAKE_EN`, `SLEEP_EN`, and `ENABLE` registers are.

## Activity

There are several ways to measure power consumption - in this lab we will be using a benchtop power supply and multimeter.
When measuring voltage, you measure the voltage _across_ to points in the circuit in parallel.
When measuring current, you measure the current _through_ the multimeter in series.
You should be familiar with the basics of using a multimeter from the prelab readings.

Throughout this lab, we'll go through the following procedure for different scenarios.

1. Turn off the power supply.
1. Disconnect the multimeter power and ground from the board.
1. Connect USB.
1. Load the code for the scenario.
1. Disconnect your USB
1. Connect the multimeter power and ground back to the board
1. Turn on the power supply.
1. Record your measurments.


## Activity
Measure the power consumption of the following scenarios.
For each scenario, create a C file with the complete setup and main function.

To eliminate any bias in the measurements, use an external LED with a known current value limiting resistor on a GPIO pin.
Don't use the built in LED, which requires the wireless module to be enabled.
If you want a serial connection, you need to use the UART pins rather than USB and access it with a serial converter.

1. Blink an LED, using `sleep_ms` to delay iterations.
1. Blink an LED, using FreeRTOS with a thread.
1. Run a busy loop to get 100% CPU utilization
    1. Place `#pragma GCC optimize ("O0")` at the top of the file to disable compiler optimizations.
    1. Run a tight loop like the one below. Multiplication typically requires large amounts of silicon, and adders with a full carry chain causes lots of activity. I don't know if these will be the worst instructions for energy usage, but this should be a decent place to start.
    ```
while(1) {
 uint32_t k;
 for (int i = 0; i < 30) {
    uint32_t j = 0;
    j = ((~j >> i) + 1) * 27644437;
    k = j;
  }
}
```
1. Set up an interrupt to toggle a GPIO in response to an external signal (like in lab 5).
    1. In the main loop to invoke the wait for interrupt instruction.
    1. Drive the interrupt pin with the signal generator.
    1. Try a couple of frequencies between 1KHz and 1 MHz.
1. Run the sleep demo. https://github.com/raspberrypi/pico-playground/tree/master/sleep/hello_sleep/hello_sleep_alarm.c
    1. More details are in section 2.11.5.1 of the RP2040 datasheet.
    1. Read through the demo source and the library code to make sure that the code matches the setup description in the datasheet
    1. https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/pico_sleep/sleep.c
1. Run the dormant demo. https://github.com/raspberrypi/pico-playground/tree/master/sleep/hello_dormant/hello_dormant_gpio.c
    1. More details are in section 2.11.5.2 of the RP2040 datasheet.

## Reference

There is no specific reference implementation, you will be using code similar to previous labs or examples provided by Raspberry Pi.

https://github.com/raspberrypi/pico-playground/tree/master/sleep/hello_dormant/hello_dormant_gpio.c

https://github.com/raspberrypi/pico-playground/tree/master/sleep/hello_sleep/hello_sleep_alarm.c
