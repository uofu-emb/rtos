# Lab 2. You Dare Mock Me?!
In the previous lab, we made our code more testable by breaking dependencies and turning them into inputs and outputs of our function. This made it easier to inject test data into our unit under test. But what if we have to call a system function or macro? How do we work with this in our test?

A common technique in unit testing is **mocking**. We replace the system function or other dependency with an implementation that we control.

# Learning objectives
* Use the C preprocessor to do conditional modification of source code.
* Understand the syntax of function pointers.
* Write function that take function pointers as arguments.
* Write unit tests in the context of libraries and other systems.
* Understand how to use mocking techniques to break dependencies for testing.
* Implement several mocking techniques.
* Measure and test the behavior of code using mocked functionality.

# Pre-lab
1. Create a new project and copy in the .c files from this lab.
1. Read through the lab and familiarize yourself with the terms used and general principle of the techniques.

# Lab
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

The preprocessor is a powerful tool. It can be used to provide alternate implementations, change behavior for different runtime or platforms, disable or enable functionality, add debugging and tracing facilities. As with any powerful tool you can also shoot yourself in the foot.

### Activity
In the given files, a simple function `say_hello`is defined which sends a string containing a traditional greeting, one character at a time over a UART device.
1. Modify the conditional_hello.c file, using the preprocessor to conditionally control the implementation of the system uart functions.
1. Create test functions that will be called instead.
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
1. Implement the mock call to uart_poll_in to return sequence of UART bytes.
1. Make sure the byte sequence matches the expected terminal behavior of the incoming data.

## Asserting functionality.

### Assertion internally

If some interaction with the dependency needs to be verified, you can make assertions inside your mock.

```
err_t do_thing(int x)
{
    TEST_ASSERT_TRUE(x == 0);
}
```

Another option is to store information about an interaction in a global variable and check its value This could be as simple as checking how many times the function was invoked, or storing the parameters passed for later inspection.
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
1. Implement the mocked UART sending behavior, asserting that the correct sequence of bytes is sent.

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
1. Copy the map.h file into your project.
1. Implement the map function.
1. Write tests.

1. Create a new version of the UART code, using function pointers for the system calls.  (be sure to use new non-conflicting function names)

# Other techniques to be aware of.

## Redefine macros

The C preprocessor allows you to do all sorts of silly things, like undefine previously defined macros.
```
#undef TOGGLE_PIN(pin)
#define TOGGLE_PIN(pin) my_toggle(pin)
```
This isn't a good idea to do in normal practice - who knows what mayhem you could cause. But in a test, it's a technique you have available manipulate the behavior.

## Change static linking
When we use a library function in C, we don't reference the implementation file, we include the header `.h` file. This tells the compiler the definition of the function __signature__, but not the implementation. At compile time, the location of the function implementation is not known. At linking time, the linker takes all of the compiled object files and links the function definition to its implementation.

If no implementation of a function exists, the code will still compile but will fail to link. Likewise if there are two implementations of a function, the linker will fail because it has two competing implementations.

We can leverage this to provide a different implementation. We create our own `.c` file containing our implementation of the function.

This technique can be very useful, but does require a bit more effort to setup. It requires modification of the build scripts to change what object files are passed to the linker. Because this involves manipulating our build scripts, we won't be tackling this particular technique in an activity, but keep it in your toolbox.

# Beware tight coupling
Rebinding and mocking is often most useful when you have high visibility and accessibility into code. The danger is making your tests too coupled to the internal implementation of the code you are testing. Unit testing in general treats the unit under test as a black box. Mocking introduces a dependency of your test to the internals of the tested code.
