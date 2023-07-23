# Signaling
In this lab we'll use shared state to transfer data between threads.
## Semaphores
For the first example, we'll use semaphores to synchronize passing off data to a worker thread. The worker performs a long running calculation. The
### Activity
1. Create a new project. Copy into your project the signals.c example code.
2. Review the contents of the file.
    1. Does the system deadlock?
    2. Does the system perform the arithmetic computation correctly?
    3. Why does the thread need to call k_yield()?
3. Write a unit test.

## Events
