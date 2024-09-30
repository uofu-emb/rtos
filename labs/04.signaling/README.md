# Learning objectives.
* Use queues as an inter-process interface
* Use semaphores to signal between processes.

# Pre-lab
In this lab we'll use communication abstractions to transfer data between execution contexts.

## Readings
Read Lee & Seshia 11.3.

Read the documention to get an overview of what data passing structures are available in FreeRTOS.

https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/01-Queues

https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/04-Stream-and-message-buffers/01-RTOS-stream-and-message-buffers

Review the API documentation to get a feel for what functions are available.

https://www.freertos.org/Documentation/02-Kernel/04-API-references/06-Queues/00-QueueManagement

## Signalling
We've used sempaphores to do locking. We can also use them to signal between processes.

```
int data;
struct SemaphoreHandle_t;

void wait_for_ready(void)
{
    xSemaphoreTake(ready, portMAX_DELAY);
    do_thing(data);
}

int mark_ready(int value)
{
    data = 42;
    xSemaphoreGive(ready);
}
```

Note the default state of the semaphore is _taken_, and it only becomes given when the data is ready. This is the opposite of the locking use which wants to pair take and give in the same context.

## Message passing
### Available data types.
https://docs.zephyrproject.org/2.7.5/reference/kernel/index.html#data-passing

Many of these data structures are applicable for purposes other that communciation. For example a stack is a familiar feature of threads, and queues are common data storage. Many have very similar behavior - the underlying structure may be implemented using another of one of these structures.

The structure behaviors fit into two broad categories: first-in/first-out (FIFO) and last-in/first-out (LIFO). This defines the ordering between items added to the structure and items removed from the structure. Two of the structures are even named FIFO annd LIFO, so be careful when you distinguish between category and the OS type.

The distinction between each of these structures is largely:
* the underlying data structure
* what type of data can be stored
* the context in which it can be used
* the behavior if the structure runs out of space.
* functionality available to the caller, for example blocking on receiving an item and atomicity.

In this lab we'll use a message queue k_msgq, which is a FIFO structure. It has a fixed width data type and internal storage. This simplifies the need to manage memory like in the case of the k_fifo

## Message passing vs shared state
One advantage of data passing is that the _producer_ side is decoupled from the _consumer_ side. Data flows in one direction, and the producer has little need to know the details of the consumer implementation. With a queue used, the producer doesn't need to wait for the consumer (bound by the storage available in the structure).

# Lab
## Semaphores
For the first example, we'll use semaphores to synchronize passing off data to a worker thread. The worker performs a long running calculation and synchronizes the response.
### Activity
1. Create a new project.
1. Copy into your project the test_signal.c test, and the signaling.h header.
1. There is also a CMakeLists.txt configuration which sets up multiple test executables you should look at.
1. Implement the worker handler and the request functions, matching the signature specified in the header file.
1. Your worker should return the input + five

## Thread pool
In the previous example, we were limited to a single worker. In the second example, we'll extend to a _worker pool_. The worker will pull data off the _task queue_, do the work, and send the results back to the supervisor on a response queue.

### Activity
1. Copy into your project the test_fifo.c test.
1. Implement the worker handler and the supervisor functions, matching the signature specified in the header file.
1. Your worker should return the input + five
