# Lab 2. You Dare Mock Me?!
In the previous lab, we made our code more testable by breaking dependencies and turning them into inputs and outputs of our function. This made it easier to inject test data into our unit under test. But what if we have to call a system function or macro? How do we work with this in our test?

A common technique in unit testing is **mocking**. We replace the system function or other dependency with an implementation that we control. We'll explore some techniques for manipulating bindings in source code.

Mocking the implementation of peripheral or system function can work for simple behavior, but for more complex behavior you might end up writing a full parallel implementation of the microcontroller environment. Instead of writing this ourselves, we'll leverage an existing tool called Renode. Renode provides an emulated environment where are given a working implementation of the system but also complete control and visibility. We'll write some tests using the simulator environment.

# Learning objectives
* Write unit tests in the context of libraries and other systems.
* Use the C preprocessor to do conditional modification of source code.
* Understand the syntax of function pointers.
* Write functions that take function pointers as arguments.
* Understand how to use mocking techniques to break dependencies for testing.
* Measure and test the behavior of code using mocked functionality.
* Write tests in the context of a simulated environment.
* Measure and test the behavior of code using a simulator.

# Pre-lab
1. Create a new project and copy in the .c files from this lab.
1. Read through the lab and familiarize yourself with the terms used and general principle of the techniques.

# Lab
## Setting up multiple tests
In this lab, we'll have several different tests which require their own setup.
When setting up a test, create a folder in `test` with the name prefixed with 'test_'. The test runner will scan the directory for anything starting with that prefix. For more information check the documentation.

https://docs.platformio.org/en/stable/advanced/unit-testing/structure/hierarchy.html
## Macro conditionals
Macros allow textual manipulation of the source code. The `#ifdef` and `#ifndef` directives allow conditional inclusion of source blocks.
```
void do_the_thing(void) {
...
#ifdef TESTING
    sys_call();
#else
    test_call();
#endif
...
}

void
```

If a macro TESTING is defined, one implementation is used. If it is not defined, the other implemntation is used.

You can use function-like macros, which take arguments.

```
#ifdef TESTING
#define compare(left, right) (left < right)
#else
#define compare(left, right) (left > right)
#endif
```

You can replace whole blocks of code.
```
#ifdef TESTING
#include "test_stdio.h"
void do_the_thing(void) {
    // Do test specific thing
}
#else
#include <stdio.h>
void do_the_thing(void) {
    // Do normal thing
}
#endif
```

The preprocessor is a powerful tool. It can be used to provide alternate implementations, change behavior for different runtime or platforms, disable or enable functionality, add debugging and tracing facilities. As with any powerful tool, you can also shoot yourself in the foot.

### Activity
In the `uart_hello.*` files, a function is defined which uses a UART device. It echoes back any character received (in uppercase), until a newline character is received.

1. Use the preprocessor to conditionally control the implementation of the system uart functions.
    1. Define new functions `test_uart_out` and `test_uart_in` in your header file. Wrap them in a conditional, testing for the definition of TESTING_ENV.
    1. Wrap the call to `uart_poll_out` and `uart_poll_in` in a preprocessor conditional
1. We'll implement the test functions next.

## Testing with mocks.
### Mocking functionality.

Once an alternate binding of the function is in place, the next question is what to do with it. If our code is dependent on the behavior of a function, we can create an implementation with controlled behavior in the test.

#### Static implementation

Sometimes behavior is simple or irrelevant enough that a static implementation is sufficient.

```
err_t do_thing(int x)
{
    return STATUS_OK;
}

float add_fpu(float x, float y)
{
    return 42;
}
```

### Internal logic

A test implementation of a function could emulate the behavior of dependency.

```
uint32_t sd_card_read(uint32_t address)
{
    switch (address) {
        case 42: return 'A';
        case 47: return 'a';
        default: return 0;
    }
}
```

## Controlled
If the behavior of the dependency changes over time, you can use a __closure__ to inject state into a function. Working within the constraints of C, this can only be done by binding a scoped variable from the global scope (In languages with first-order functions, we could create an anonymous function that __closes over__ a variable in the current scope.)

```
int flag = 0;

err_t do_thing(int x)
{
  if (flag == x) return 0;
  else return 1;
}

...
void test_thing(void)
{
...
    flag = 0;
    TEST_ASSERT_TRUE(do_thing(0) == 0);
    flag = 1;
    TEST_ASSERT_TRUE(do_thing(0) == 1);
...
}
```

The use of global can be used to change behavior across multiple invocations.
```
int flag = 0;
int do_thing(int x)
{
    return flag++;
}

...
void test_thing(void)
{
...
    TEST_ASSERT_TRUE(do_thing(0) == 0);
    TEST_ASSERT_TRUE(do_thing(0) == 1);
...
}
```

### Activity
1. In your test code, define TESTING_ENV.
1. Implement the `test_uart_in` and `test_uart_out` functions in your test code.
1. Implement the `test_uart_in` to return sequence of UART bytes.

## Asserting functionality.

### Assertion internally

If some interaction with the dependency needs to be verified, you can make assertions inside your mock.

```
err_t do_thing(int x)
{
    TEST_ASSERT_TRUE(x == 0);
}
```

Another option is to store information about an interaction in a global variable and check its value. This could be as simple as checking how many times the function was invoked, or storing the parameters passed for later inspection.
```
int do_thing_invoked = 0;
int last_param;
err_t do_thing(int x)
{
    last_param = x;
    return do_thing_invoked++;
}

...
void test_thing(void)
{
    ...
    do_thing_invoked = 0;
    do_thing(42);
    TEST_ASSERT_TRUE(do_thing_invoked == 1);
    TEST_ASSERT_TRUE(last_param == 42);
    ...
}
```
#### Activity
1. Implement the `test_uart_out` UART sending behavior, asserting that the correct sequence of bytes is sent.

## Manipulating bindings with function pointers
Recall that we operate under a Harvard architecture: Instructions are stored in memory just like any other data. So like any other data, we can reference it, and create pointers to it.

### Polymorphism isn't just for object oriented programming.

Let's imagine I am sorting an array of integers. In my first implementation I compare two elements and would pick the smaller of the elements to go first. This sorts the list in ascending order.

```
void sort(int32_t *array, int length)
{
 ...
 // Which element comes first?
 if (left < right) pick left;
 else if (right < left) pick right;
 else pick left;
 ...
}
```

This works great until I have to sort my list in descending order. Now I could copy all of the code into a new function and just change those three lines, but we can be trickier than that.

What we really need is a standard way to compare two numbers and return what order they go in.

We have three cases:
If the left comes before the right, we return 1.
If the right comes before the left, we return -1.
If they are the same, return 0.

Let's define an implementation for the ascending case.
```

int ascending(int32_t left, int32_t right) {
    if (left < right) return -1;
    else if (left > right) return 1;
    else return 0;
}

```
Now that we have a function, how do we pass a reference to the `sort` function? Same way we pass reference to data, with a pointer!

```
void sort(int32_t *array, int length, int (*cmp)(uint32_t, uint32_t))
{
...
if (cmp(x, y) > 0 ) pick x;
else pick y;
...
}

int32_t *a = ...;
sort(a, A_SIZE, ascending);

```

Now the sort function is able to operate without caring __how__ the comparison is implemented. As long as the function we pass meets the signature expected, we can pass it as the comparison function. This is known as **polymorphism**. You have likely encountered this concept in object oriented programming languages like Java, where polymorphism is implemented via inheritance and interface implementation. There are many different ways to implement polymorphism, don't let the OOP languages tell you they have a monopoly on it.

Now if my boss tells me that on Tuesdays we do things backwards, I can dynamically change the implementation at runtime. This is sometimes called __late binding__ or __dynamic binding__.

```
int descending(int32_t left, int32_t right) {
    return -ascending(left, right);
}

uint32_t (*todays_comp)(int32_t);

if (today == TUESDAY) todays_comp = descending;
else todays_comp = ascending;

sort(a, A_SIZE, todays_cmp);
```
### Drawbacks
One drawback of this approach is that you have not statically defined the function you will be calling. This means the compiler cannot tell what implementation of a function is being used at compile time. Compilers are able to leverage static information for verification and optimization, which is no longer possible.

This pattern is the most invasive - it requires refactoring out major portions of the code. In the sorting example, the capability of passing in a comparison function was useful even if we weren't testing the code. Any caller will have to provide their own function pointer input. This can cause a cascade of input definitions as each function requires a pointer to pass to the next layer. This can show you just how prevalent a dependency really is, but it becomes inconvenient in practice.

#### Activity
The higher order function `map` takes as input a list or other container, a function to apply to each element, and returns a list of the same size after applying the function to each element.
1. Copy the test_map.c file into your project.
1. Implement the map function.

#### Activity
1. Create a new version of the UART code, using function pointers for the system calls.  (be sure to use new non-conflicting function names)


## Change static linking
When we use a library function in C, we don't reference the implementation file, we include the header `.h` file. This tells the compiler the definition of the function __signature__, but not the implementation. At compile time, the location of the function implementation is not known. At linking time, the linker takes all of the compiled object files and links the function definition to its implementation.

If no implementation of a function exists, the code will still compile but will fail to link. Likewise if there are two implementations of a function, the linker will fail because it has two competing implementations.

We can leverage this to provide a different implementation. We create our own `.c` file containing our implementation of the function.

Recall that in our project structure, code in `lib` can be used in both test and runtime environments. The code in the `lib` directory is split into modules.

### Activity
1. Create a new test, and implement `uart_poll_in` and `uart_poll_out` functions directly in the test.

## Other techniques to be aware of

### Redefine macros

The C preprocessor allows you to do all sorts of silly things, like undefine previously defined macros.
```
#undef TOGGLE_PIN(pin)
#define TOGGLE_PIN(pin) my_toggle(pin)
```
This isn't a good idea to do in normal practice - who knows what mayhem you could cause. But in a test, it's a technique you have available to manipulate the behavior.

### Beware tight coupling
Rebinding and mocking is often most useful when you have high visibility and accessibility into code. The danger is making your tests too coupled to the internal implementation of the code you are testing. Unit testing in general treats the unit under test as a black box. Mocking introduces a dependency of your test to the internals of the tested code.

# Simulation
In the previous section we did some rebinding tricks to try and get visibility into the behavior of our system. This worked, but there were some drawbacks. Let's examine a different way to control and instrument the environment of our system.

Our tests so far have focused on testing small units of functionality. We wrote test code using the Unity testing framework, which was designed to run on the microcontroller self-contained. Our tests worked on individual functions. Let's instead think of our program holistically as a system which is interacting with some external environment.

https://renode.readthedocs.io/en/latest/introduction/using.html

https://docs.platformio.org/en/stable/advanced/unit-testing/simulators/renode.html

## Interacting with Renode
### Activity
Let's run renode and explore the functionality. Launch renode without any arguments.

To start we need to create a machine.
```
(monitor) mach create
```
Note the prompt changes to show our context. Next we load in the hardware description. The simulator includes specific configuration for the discovery board. The @ sign refers to a file, which in this case it scans for files and finds on relative to the renode installation directory.
```
(machine-0) machine LoadPlatformDescription @platforms/boards/stm32f072b_discovery.repl
```
Next we need to load in the binary file (remember to compile first). This scans the relative path to the current directory.
```
(machine-0) sysbus LoadELF @.pio/build/disco_f072rb/firmware.elf
```
Finally show the usart1 interaction window and start the simulation.
```
(machine-0) showAnalyzer sysbus.usart1
(machine-0) start
```

You should be able to type in the interaction terminal, and your system will echo the text you type.

The hardware description sets up a series of device objects. If you specify an object with now function or member calls, it will show the documentation for that object.
```
(machine-0) sysbus.usart1
```

Let's write a small interaction that should blow up if we get an unexpected result.
```
sysbus.usart1 AddLineHook "HELLO" "assert(line == 'HELLO WORLD!')"
sysbus.usart1 WriteLine 'hello world!'
sysbus.usart1 DumpHistoryBuffer
```

You can put all of this into a script and run it from `renode script.repl`.


# **Skip this for now, the installed renode package has a broken version of renode-test**

## Automating Renode tests with Robot Framework.
With the simulation running, we would like to run a series of interactions and measure the behavior of the system.
The Robot Framework provides a __domain specific language (DSL)__ which is specific to the Renode environment.

### Activity

1. Copy the platformio.ini file from this directory.
1. Copy the uart_echo.robot file into your test directory.
1. Create a test which invokes echo_uppercase once.
1. run `pio test --environment disco_f072rb --without-uploading`.

Let's examine the contents of the robot file.

The robot file uses a set of __keywords__, which work like functions. In this file we define a custom keyword which sets up a machine. You'll note that this uses the "Execute Command" keyword, which is provided by renode. This executes the same commands from our interactive session.
```
*** Keywords
Create Machine
    [Arguments]  ${elf}

    Execute Command          mach create
    Execute Command          machine LoadPlatformDescription @platforms/boards/stm32f072b_discovery.repl
    Execute Command          sysbus LoadELF ${elf}
```

The next section is our actual tests. The first line specifies the name or description of the test. The remaining lines are a set of keywords.
```
*** Test Cases ***
Should Run Zephyr Console Echo Sample
    Create Machine           '${CURDIR}/../.pio/build/disco_f072rb/firmware.elf'
    Execute Command          showAnalyzer ${UART}
    Create Terminal Tester   ${UART}

    Start Emulation
    Write Line To Uart     hello world!
    Wait For Line On Uart  HELLO WORLD!
```
One tricky gotcha is that keyword arguments need more than 1 whitespace to separate them from the keyword.

1. Create a new test case in the robot file which passes a different string.
