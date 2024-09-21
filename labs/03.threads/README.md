# Lab 3 Threads and shared state.
# Learning objectives:
* Setup the operating system.
* Setup multiple threads.
* Identify shared state and race conditions.
* Protect critical sections.
* Write unit tests.

# Pre-lab
## Readings
Read the FreeRTOS thread, semaphore, and mutex documentation (you don't need to read the scheduling, queue, or co-routine sections, we'll cover scheduling and queues in later labs).

https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/00-Tasks-and-co-routines

https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/02-Binary-semaphores

Read sections 11.1 and 11.2 from Lee & Seshia.

https://ptolemy.berkeley.edu/books/leeseshia/releases/LeeSeshia_DigitalV2_2.pdf

Skim the API reference documentation to get an idea of what functions are available.

https://www.freertos.org/Documentation/02-Kernel/04-API-references/01-Task-creation/00-TaskHandle

# Additional reading

Before the second day of lab, read sections 1-3 of this paper.

https://www.sciencedirect.com/science/article/pii/S1071581907000456

## Background.
This is an overview of the basic concepts that will be used.
#### Threads/tasks
A **thread** or **task** is an __execution context__ managed by the operating system. FreeRTOS calls them tasks. Different libraries will use one term or the other (or both), and we'll be using them interchangeably in this class.  The normal execution contexts for an embedded system running on bare metal are the main function execution and interrupt handlers. Threads have more in common with the main function than with interrupts - they typically run forever in a lop, and don't preempt the system in reaction to some event. The main function execution context is often called the __main thread__.

In FreeRTOS, the structure that represents a thread is `TaskHandle_t`.

#### Semaphores and Mutexes
Semaphores are often used as inter-context signalling mechanisms. There are many possible ways to use them - in our case we are using them as a way to create mutually exclusive (__mutex__) access around a shared state or resource. This is often simply called a __lock__.

You'll often hear the terms mutex, lock, and semaphore used interchangeably. In FreeRTOS, a k_semaphore and a k_mutex are separate types with subtle difference in behavior. A FreeRTOS mutex is __reentrant__, which means a thread can take the mutex mutiple times (as long as it gives it the same number of times). It also manipulates priorities of the schedule. It should __not__ be used in an ISR. We'll use semaphores in this lab.

Semaphores have a count, which represents the available resources. Semaphores have two operations, incrementing or decrementing the count.
The operations are traditionally denoted as P and V, Dijkstra's earliest paper on the subject gives the Dutch terms __passering__ ("passing") as the meaning for P, and __vrijgave__ ("release") as the meaning for V.
Alternate terms include down/up, wait/signal, acquire/release, and procur/vacate. In the FreeRTOS OS library the operation is **take** and **give**.

When a process wants access to the resource(s), it attempts to __take__ the resource. If the count is zero, the process will need to wait until a resource is available. If the count is greater than zero, there are available resources, and the counter is decremented.  When the process is finished with the resource, it __gives__ the resource back, decrementing the counter.

The operating system handles the internal concerns of atomicity, tracking counter value, as well as blocking and waiting for availability.

## Shared state
The execution contexts in this program both use two shared resources. The first is __shared state__, a global variable `counter` containing a count. The second is a shared system device, the standard IO `printk` function and its underlying serial output. These resources are not __thread-safe__, and cannot be accessed by more than one thread at a time. Anytime we access a shared resource we must do so in a mutually exclusive way.

## Critical sections
A __critical section__ refers to a block of code that **must** be executed mutually exclusive with other execution contexts that share resources.

Critical sections should be small as possible - while in the critical section, other execution is halted. Like interrupt handlers, staying in a critical section for too long can __starve__ the system. We'll discuss this topic in more detail in later labs. For now, any independent activity of the thread should be outside the bounds of the protected critical section

## Race conditions
A __race condition__ is a scenario in which two threads have behavior that depends on the execution order of their code. If a thread can be preempted, it can and will be preempted at any point, even in the middle of a line of C code. You should never rely on timing or chance for your code to work.

### You won't believe this one weird style trick
In C, you can always create a block by wrapping code using curly braces. We can use this as a way to visually distinguish and organize our critical sections. Failing to unlock or release a resource is a common problem, so we want to be able to quickly verify that every take has a matching give.
```
        k_sem_take(&semaphore, K_FOREVER);
        {
            counter = counter + 1;
        }
        k_sem_give(&semaphore);
```
This creates a visual distinction of what is "inside" vs "outside" of the context of the lock. We already use braces and indentation to distinguish conditional and looping constructs, so this is a natural extension. This is a first class pattern in Python for example, using `with` blocks to provide a managed context for an open resource.

I've never seen anyone else do this but me, but I think it is a good way to organize your code. Hopefully it catches on.

# Lab
## Working with threads.
### Activity 0
1. Create a new project.
2. Copy in the `thread.c` example file. Commit.
3. Identify the execution contexts in the program, and their entry points.
4. Identify shared state between execution contexts.
5. Identify the semaphore.
6. Predict the behavior of the program.
7. Run the program and compare the output to your prediction.
    1. The easiest way to do this is to copy the file into `test`, remove the `--disable-xwt` argument in platform.io and run the `pio test` command.

### Activity 1
1. Are all uses of the shared resources in protected critical sections? Make any modifications necessary to protect the critical sections.
2. Is the critical section starving the system? If so, make modifications to prevent starving the system.

## Testing threaded code
Testing threaded coded is hard. Our test methodology so far relies on one execution context.
1. If we wanted to run multiple threads during a test, our test would need some way to synchronize the state of the threads with the test.
2. The operating system manages the execution of threads. Just like interrupts, the execution of a thread can be preempted. The behavior and timing of the operating system thread management is __stochastic__, meaning that anything could happen at anytime. You should never rely on timing or chance for your code to work.
3. If we did try and test for race conditions, the test now becomes probabilistic. One invocation of the test might fail, the next may not.
4. We don't want to test the library code, it's already tested for us. Setting up and running threads is invoking library code.
5. Threads usually are long running, often with an infinite loop.
6. Threads may block indefinitely waiting for a resource.

### Activity 2
1. Write a unit test for the thread code. Remember to separate functionality from execution context concerns, timing, and looping.
1. You'll need to make some modifications to the code. Hints:
    1. You don't need to test execution in a thread, but you should test the behavior of the lock and the side effect.
    1. k_semaphore_take includes a timeout on waiting for the semaphore.
    1. k_semaphore_take returns a status code, don't forget to check it.
1. Don't forget to commit as you go.

# Deadlock.
__Deadlock__ is a condition when one thread holds a lock and is incapable of releasing it.
Let's examine two possible cases of deadlock.
## Case 1
There are two threads, and two locks.
One thread has lock A, and is waiting for a lock B. The other thread holds lock B and is waiting for resource A.

### Activity 4
1. Write code that creates this situation.
2. Write a test that shows this code will lock.
    1. You'll need to have your test wait for a short period of time, check the state of the threads, and then kill them with k_thread_abort.
    1. Hint: you might find it useful to pass semaphores or other data into the thread with the three available p1, p2, and p3 general parameters passed to the thread entry function.
3. https://docs.zephyrproject.org/2.7.5/reference/timing_functions/index.html

## Case 2, the orphaned lock.
A thread acquires a lock but fails to properly release it.
```
void orphaned_lock(void)
{
    while (1) {
        k_semaphore_take(&semaphore, K_FOREVER);
        counter++;
        if (counter % 2) {
            continue;
        }
        printk("Count %d\n", counter);
        k_sem_give(&semaphore);
    }
}
```
### Activity 5
1. Write a test for the functionality of the thread.
2. Write a test showing the thread will deadlock.
    1. Recall your test will need to kill the deadlocked threads before it can complete.
3. Create a new version of the code that will not deadlock.
4. Write a test showing the thread will not deadlock.
