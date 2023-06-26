# Learning objectives:
Organize a project.
Read and document unfamiliar code.
Navigate documentation.
Write unit tests.
Write a manual test procedure.

Setup the operating system.
Setup multiple threads.
Identify race conditions.
Protect critical sections.
# Prelab
## Identify the different components of the PlatformIO organization
https://docs.platformio.org/en/latest/platforms/index.html
Platforms - the microcontroller architecture
https://docs.platformio.org/en/latest/frameworks/index.html
Framework - the operating system or core HAL libraries
https://docs.platformio.org/en/latest/librarymanager/dependencies.html
Package - an installable library
https://docs.platformio.org/en/latest/boards/index.html
Board - a configuration for a specific development board.
## platformio.ini Configuratino file
https://docs.platformio.org/en/latest/projectconf/index.html
## Zephyr documentation
https://docs.zephyrproject.org/latest/doxygen/html/index.html

# Lab
## Background.
This is an overview of the basic concepts that will be used
#### Threads
k_timer

#### Semaphores
k_sem

#### Timers
k_thread



## Instantiate the project
You should have a project setup after lab 0. For reference, here is a quick setup.

Setup the project with `pio project init --board nucleo_f446re --project-option "framework=zephyr"`
Init a git repository with `git init`
Copy in the github actions template **todo add a link** to `.github/workflows/main.yml`
Copy in the .gitignore template **todo add a link** to .gitignore
## Working with threads.
### Setup
Copy the thread.c function from this directory into your project src directory. We will be using this file as the foundation of the lab.

Commit the new main.c file to source control. Make sure to give a helpful and descriptive commit message.
### Working with undocumented code.
You now have the all too common task of taking an undocumented piece of code, understanding it, and modifying it.

There are many ways to approach an unfamiliar codebase. Some strategies you might take:
- Find the main function and follow the execution of the code.
- Run the code.
- Write tests.
- Start at the top of the file and scan through the file.
- Read the tests for the code.
- Look at documentation or comments.
- Step through execution with a debugger.
- Identify variables and data structures used by the code.
- Identify what functions are defined in the file.
- Identify library and external function calls and look up their documentation.
- Add comments as you read.
- Take notes on paper.
- Draw diagrams.
- Compare with other code or examples.
- Ask someone familiar with it.
- Refactor the code.
- Explain to another person or rubber duck how it works.

Some code may be better approached with different strategies than others. For example, one big long function might require a different approach than a file broken up into multiple functions.

When faced with a large system, you may not need to understand every line!

Activity:
Discuss with your partner how to approach this particular code. Come up with a brief plan.

You may find a particular pattern or strategy works well for you. Discuss with your partner if their preferred strategy is the same or different from yours.

Discuss with your partner what type of code might cause your preferred strategy to break down.

### Commenting the code
Adding documentation is important for both your sanity and the sanity of the person who has to work with this code next.

Not every line needs a comment! Comments should be descriptive and explanatory. Avoid comments that simply restate what is in the next line of code. Give context and reasoning behind the why the code is there.

Activity:

As you do your exploration of the code, add comments to the file explaining what it does, how it works, how it is organized, etc.

You do not have to understand perfectly what is going on to proceed. Once you have a rough idea and added comments, commit the changes to the files.

### Exploratory surgery.

Sometimes the best way to understand how things work is to take them apart and break stuff.

Activity:
If you have not already done so, flash the code to your microcontroller and observe the behavior of the system.

Delete or comment out the calls to `k_sem_take` and `k_sem_give` (make sure you have committed prior to this!). Predict what you think will happen.

Compile your changes and flash the micontroller. Does the behavior match your prediction?

Restore your previous code. You can use the command `git checkout src/main.c` to restore the file to the current committed version of the file.

## Write a test and making a change.
"Code without tests is bad code. It doesn’t matter how well written it is; it doesn’t matter how pretty or object-oriented or well-encapsulated it is. With tests, we can change the behavior of our code quickly and verifiably. Without them, we really don’t know if our code is getting better or worse."

Michael C. Feathers, Working Effectively with Legacy Code

Before we make a change, we should write some tests. Manual testing can be fast, but has the issue of repeatability and time to test. Once the system grows in size, manually testing system becomes infeasible.

### Manual testing - quality assurance
Manual testing is still a valuable tool available to you, but needs to be approached systematically. Software teams often include team members doing quality assurance (QA). This umbrella term encompasses a wide range of techniques and activities. For this exercise, we will develop a __regression test__. The purpose is to verify that a change to the system does not affect previous expected behavior.

Tests need to be repeatable. They also need to be documented so that new members of the team can . Documenting the expected behavior of the system also provides a record for those making changes to understand how the system works.

Let's develop a testing plan. The testing plan should have three sections:
1. How to set up the test scenario.
2. How to exercise the system
3. Expected behavior.

Activity:

Write down a test plan in a file tests/manual/something.md.

Commit it to source control.

Work with another group. Follow their test plan. Discuss with them your experence. Was there anything unclear or missing? Does the plan describe the behavior of the system correctly? Are there any edge cases the test plan would miss?

### Automated testing strategy

What inputs are there to the system?

In our case, there are not any inputs to our system. If there were inputs, our test would need to include multiple cases of different input combinations.

What state is there? How does it change.

__State__ refers to properties or data of the system that change over time. State can also be thought of as time dependent inputs and outputs to your system. In our case, there is the state of the semaphore, and the state of the timer.

What execution contexts are there?

There are two execution contexts, the thread and the main loop. Execution contexts can execute in

What outputs can we observe?

In this case, the system prints out a message to the console. Could we leverage this as a measurable output?

What dependencies are there?

Dependencies refer to systems and behaviors external to your code that your code relies on the behavior of. Dependencies present a

### Unit vs. Integration testing
Unit tests cover the smallest functional unit of your system. They are usually state invariant. Any initial state is set ahead of time to a known value, and any changes in state are solely due to the execution of the code. Unit tests usually have an expected output for a given input, and teh output is __asserted__ to be the expected value. Unit tests should have limited references to dependencies, and those dependencies either need to be controlled or __mocked__ with a simulated implementation. Unit tests should be automated and fast to run.

Integration tests cover the behavior of the system in aggregate. This measures interactions between units you tested via unit tests. Integration testing is complicated by the large scope and the integration of dependencies into the system under test. Integration testing is often done manually, simulated, or run on actual hardware.

###
The `printk` function outputs a kernel message. We can capture that


https://docs.platformio.org/en/latest/advanced/unit-testing/index.html
https://docs.platformio.org/en/latest/advanced/unit-testing/frameworks/unity.html
# Post-Lab
