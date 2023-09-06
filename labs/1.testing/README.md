# Lab 1. Writing testable code

# Learning objectives:
* Organize a project.
* Read and document unfamiliar code.
* Navigate documentation.
* Write unit tests.
* Run tests in simulation
* Run tests on hardware
* Write a manual test procedure.
* Refactor for ease of testing.

# Prelab
"Code without tests is bad code. It doesn’t matter how well written it is; it doesn’t matter how pretty or object-oriented or well-encapsulated it is. With tests, we can change the behavior of our code quickly and verifiably. Without them, we really don’t know if our code is getting better or worse."

Michael C. Feathers, Working Effectively with Legacy Code

## Identify the different components of the PlatformIO organization
Platforms - the microcontroller architecture
https://docs.platformio.org/en/latest/platforms/index.html

Framework - the operating system or core HAL libraries
https://docs.platformio.org/en/latest/frameworks/index.html

Package - an installable library
https://docs.platformio.org/en/latest/librarymanager/dependencies.html

Board - a configuration for a specific development board.
https://docs.platformio.org/en/latest/boards/index.html

## platformio.ini Configuration file
https://docs.platformio.org/en/latest/projectconf/index.html

# Lab
## Instantiate the project
You should have a project setup after lab 0. For reference, here is a quick setup.

1. Setup the project with `pio project init --board nucleo_f446re --project-option "framework=zephyr"`
1. Init a git repository with `git init`
1. Copy in the github actions template to `.github/workflows/main.yml`
1. Copy in the .gitignore template to `.gitignore`

## Project structure

PlatformIO has an opinionated project structure. It's important to put files in the correct locations so that the build system can properly find them.

1. Put all your system glue code and configuration in `src` and `include`.
    1. Code in these directories are not available when running tests.
    1. You can only have one definition of `main`.
1. Put all of your components and logic into `lib`
    1. Inside `lib`, create one or more component directories.
    1. Inside your component directory, create a `src` and `include` directory.
    1. You should have `lib/mycode/src` and `lib/mycode/include`
1. Put tests in `test`.
    1. Multiple tests can be set up in subdirectories.

More information:

https://docs.platformio.org/en/latest/advanced/unit-testing/structure/hierarchy.html

### Activity
1. Copy the blink.c function from this directory into your project `src` directory. We will be using this file as the foundation of the lab.
1. Create two directories `lib/lab1/src` and `lib/lab1/include`
1. Copy the `unity_config.h` and `unity_config.c` files into `test`.
1. Create a new file `zephyr/prj.conf`. Add the following.
```
CONFIG_SERIAL=y
CONFIG_NEWLIB_LIBC=y
```
Commit the new files to source control. Make sure to give a helpful and descriptive commit message.

## Working with untested code.
### Tactics
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

#### Activity:
Discuss with your partner how to approach this particular code. Come up with a brief plan.

You may find a particular pattern or strategy works well for you. Discuss with your partner if their preferred strategy is the same or different from yours.

Discuss with your partner what type of code might cause your preferred strategy to break down.

### Commenting the code
Adding documentation is important for both your sanity and the sanity of the person who has to work with this code next.

Not every line needs a comment! Comments should be descriptive and explanatory. Avoid comments that simply restate what is in the next line of code. Give context and reasoning behind the why the code is there.

#### Activity:

As you do your exploration of the code, add comments to the file explaining what it does, how it works, how it is organized, etc.

You do not have to understand perfectly what is going on to proceed. Once you have a rough idea and added comments, commit the changes to the files.

### Exploratory surgery.

Sometimes the best way to understand how things work is to take them apart and break stuff.

#### Activity:
If you have not already done so, flash the code to your microcontroller and observe the behavior of the system.

Delete the calls to `k_timer_status_sync` (make sure you have committed prior to this!). Predict what you think will happen.

Compile your changes and flash the micontroller. Does the behavior match your prediction?

Restore your previous code. You can use the command `git checkout src/main.c` to restore the file to the current committed version of the file.

## Write a test and making a change.
Before we make a change, we should write some tests. Manual testing can be fast, but has the issue of repeatability and time to test. Once the system grows in size, manually testing system becomes infeasible.

### Manual testing - quality assurance
Manual testing is still a valuable tool available to you, but needs to be approached systematically. Software teams often include team members doing quality assurance (QA). This umbrella term encompasses a wide range of techniques and activities. For this exercise, we will develop a __regression test__. The purpose is to verify that a change to the system does not affect previous expected behavior.

Tests need to be repeatable. They also need to be documented so that new members of the team can follow them. Documenting the expected behavior of the system also provides a record for those making changes to understand how the system works.

Let's develop a testing plan. The testing plan should have three sections:
1. How to set up the test scenario.
2. How to exercise the system
3. Expected behavior.

#### Activity:

Write down a test plan in a file, for example `tests/manual/something.md`

Commit it to source control.

Work with another group. Follow their test plan. Discuss with them your experence. Was there anything unclear or missing? Does the plan describe the behavior of the system correctly? Are there any edge cases the test plan would miss? How long does it take to go through the test plan?

### Automated testing strategy

When approaching how to test a system, consider the following components and concerns.

#### What inputs are there to the system?

Inputs are data used by your system. Inputs are __invariant__ or __constant__ - if they are not they are instead __state__. If there are inputs, our tests would need to include multiple cases of different input combinations.

#### What state is there? How does it change.

__State__ refers to properties or data of the system that change over time. State can also be thought of as time dependent inputs and outputs to your system. If some state is modified by calling a function, this is called a __side effect__. Side effects can often be insidious and unexpected by a caller.

#### What outputs can we observe?

Outputs generally are the results of a system operating, typically the return value of a function or procedure. They differ from state in that they are __invariant__ over time.

#### What execution contexts are there?

There are two execution contexts, the thread and the main loop. Execution contexts represent a computational environment where code runs. These are usually managed by the hardware or by the operating system.

#### What dependencies are there?

Dependencies refer to systems, devices, and behaviors external to your code that your code relies on. Dependencies present a particular challenge because they have their own behavior, state, and context to account for.

### Unit vs. Integration testing
Unit tests cover the smallest functional unit of your system. They are usually state invariant. Any initial state is set ahead of time to a known value, and any changes in state are solely due to the execution of the code. Unit tests usually have an expected output for a given input, and the output is __asserted__ to be the expected value. Unit tests should have limited references to dependencies, and those dependencies either need to be controlled or __mocked__ with a simulated implementation. Unit tests should be automated and fast to run.

Integration tests cover the behavior of the system in aggregate. This measures interactions between units you tested via unit tests. Integration testing is complicated by the large scope and the integration of dependencies into the system under test. Integration testing is often done manually, simulated, or run on actual hardware.

How big is a unit? It should be small enough that the behavior can be described in without writing a novel. It should be large enough to be non-trivial.

### Test strategies
The code is not currently written in a way that is testable. We will need to refactor the code to get into into a testable structure. Refactoring needs to be approached carefully - we must avoid changing the behavior of the code we are testing.

As you become familiar with how tests are written, you will start writing code in a way that is easier to test.

#### Separate execution context from behavior.
- Separate the concern of __how__ something is executed from __what__ is executed.
- Testing across execution contexts is tricky. Running multiple execution contexts in a test usually starts getting into integration test territory.
- Threads, interrupts, and tasks can be assumed to be working library code.
- Most code can be run in any execution context. Even something like an interrupt handler can separate concerns such as acknowledging the interrupt and the execution of your logic.
- Something that is executed in two execution contexts can usually be emulated in one context.
- You can call interrupt handlers and thread entry points - they have no special meaning in other contexts. The `main` function is different - it has a special meaning in the language. Our tests will have their own `main` function being the entry point to running the test; so you need to pull any code you are testing out of `main`.
#### Technique and Activity
1. Find the main function, interrupt handlers, and thread entry points.
2. Identify the behavioral code in these contexts. We will refactor this code in the next activities.

#### Separate iteration from functionality.
- Move the body of loops into a function.
- This is especially important for infinite execution loops. You can't really test something that never halts.
- Optionally, for collections, this separates the concern of what to do with one item from the concern of working with a collection.

##### Technique and Activity

1. Find any infinite loops in the code.
2. Identify the difference between setup and repeated execution.
3. Identify any time dependent behavior, especially delays between iterations.

#### Decompose code into functions.
- Blocks of code can't be tested. Unit tests will need to execute the code, so it needs to be in  functions.
- Functions provide clear boundaries and contracts.
- Your code should be well organized anyway!
- Not all functions are complete independent units. You may write a test that uses more than one function.
    - For example, you could write a test that pushes a value onto a queue, and then pops the value using two calls.

##### Technique and Activity
1. **Make sure all code is committed before you make any changes**.
    1. We need to make sure we know what the code was before our changes.
    2. Use `git diff` or `git difftool` to compare with the previous state.
    3. If things go wrong, you can always revert back to the working state. (Use `git checkout -- file.c`)
2. Create a new header file in the `lib/lab1/include` directory to hold the definition of your function.
3. Create a new code file in the `lib/lab1/src` directory to hold the implementation of your code.
4. Take a block of code you identified in the infinite loop of your main execution context. Create a function in your new files, and move all of the code in a block into it.
5. Put a call to the function where the code used to be.
6. Compile the code. You will be missing includes, variables, references, outputs, etc. We will deal with this in the next activity.
    1. You can use the compiler as a guide for refactoring.
    2. Some IDEs even provide automatic refactoring.

#### Convert state and dependencies to inputs and outputs.
- Make all dependencies, state, and data function inputs and outputs.
     - This helps make the code transparent to the caller of the function.
     - This shows you what you'll need to setup in your test, and what you need to assert.
     - If the inputs and outputs of your function are too numerous, your function might be doing too many things. It might be time to consider decomposing further.
- Pass pointers instead of using global variables and references.
- Mark all inputs `const` if they will not be modified.
    - Anything not marked `const` should be considered by the caller as potentially modified by the function.

##### Technique and Activity
1. For each missing variable or reference in the function you created, add an input to the function.
2. If you need to return more than one value, pass it as an __out param__, which is a pointer to a value in the caller that the function will populate.
3. Once your function is compiling, switch to the original location it was called from and populate the parameters of the function.

#### Break dependencies and separate concerns.
- Pass dependencies as inputs. Setup dependencies outside of your functions and pass them in. This is often referred to as __dependency injection__.
- Functions should do one thing and do it well.
    - If you find you have an exponential number of test cases, consider decomposing.
    - Your function may also be more appropriately treated as integration test instead of unit.
- Don't test external libraries. You can assume that it works.
- Work with data instead of side effects.
    - For example, instead of sending a string over a serial line and trying to test the serial line, return the string and test the string. There are two concerns - what the data is, and how it is sent.
- System and library function calls present a special challenge - they are __statically__ bound to identifiers at compile time. We will address this in later labs.

##### Technique and Activity
1. In the function you extracted, identify any references to global variables, HAL devices, system functions, etc.
2. See if you can remove the dependency by separating the concern of your code from the behavior of the dependency. For example, you could move a call to get data from a peripheral to the caller and pass the data into your function instead.
3. Create new inputs to your function for pointers to dependencies.

#### Write the test
Now that you have a function extracted, it is time to write the test.

##### Technique and Activity
1. Make sure your code compiles.
1. Commit the code you extracted.
1. Add the following lines to your `platformio.ini` file.
```
[env:disco_f072rb]
platform = ststm32
board = disco_f072rb
framework = zephyr
lib_deps =
    lab1
platform_packages =
    platformio/tool-renode
test_testing_command =
    ${platformio.packages_dir}/tool-renode/renode
    --disable-xwt
    -e mach create "stm32f072b"
    -e machine LoadPlatformDescription @platforms/boards/stm32f072b_discovery.repl
    -e sysbus LoadELF @${platformio.build_dir}/${this.__env__}/firmware.elf
    -e start
```
    1. We'll be running our code in the Renode simulator, so we can run our tests in GitHub actions.
    1. We can also run tests on the microcontroller.
1. Create a new file in the `test` directory.
    1. It can have any name, but by convention it has a similar name to the file you put your code in.
    1. This can make it easier for another person to find the test later.
    1. Add the code template from `test.c` in this directory to the file.
    1. Your test must include a `setUp` and `tearDown` function. You can put any code in here that will be run at before and after each of test.
    1. Your tests must have a `main` function.
        1. This sets up the testing framework with the `UNITY_BEGIN` and `UNITY_END` macros.
        1. For each test, you will call the `RUN_TEST` macro.
1.
1. Remove the example tests and add your own.
    1. Tests by convention start with `test_` but don't need to.
    1. Each test should generally demonstrate one possible case. Having multiple tests instead of one big one makes it easier to identify what is failing.
1. In your test, use any one of the GPIO peripherals available.
1. Exercise the function under test and check the state of the GPIO peripherals.
    1. Hint: check the docs at https://docs.zephyrproject.org/2.7.5/reference/peripherals/gpio.html for what functions are available to check the state of the GPIO peripheral.
1. If you find yourself having trouble setting up the test, you have too many parameters, or a dependency is getting in the way, try refactoring the function further. Follow the same techniques from the previous activities.

For more information, see the Unity documentation

https://docs.platformio.org/en/latest/advanced/unit-testing/frameworks/unity.html

Unity provides a huge variety of assertion macros:

https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md

https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsCheatSheetSuitableforPrintingandPossiblyFraming.pdf

#### Running the test


#### Not everything needs a test.
- Don't test external libraries.
- Some things are trivial and don't need tests. If you aren't sure, write a test.
- If something is not tested and breaks, write a test.
- Unit tests generally try to test your logic and computations, interaction with the system peripherals and OS can often be better tested with a simulation or running on the device.
https://docs.platformio.org/en/latest/advanced/unit-testing/index.html

#### Rinse & repeat
1. Commit your changes.
2. Continue refactoring and adding tests until you are satisfied with the test coverage.

# Examples
The directory `demo` contains a working project structure with no dependencies on Zephyr.

The directory `reference` contains a working reference implementation of the lab.
