# Lab 6. Scheduling

## Learning objectives.
Evaluate production/consumption ratio to prevent overflow.
Classify timing requirements as hard, firm, or soft real-time requirements
Prioritize tasks based on timing requirements.
Identify cases of priority inversion.
Identify the difference between cooperative and preemptive prioritization.
Identify reschedule points in the operating system.
Analyze and select scheduling algorithms based on their ability to meet real-time requirements
Build systems that meet hard timing requirements.

## Pre-lab
Watch this video about producer/consumer throughput.

https://www.youtube.com/watch?v=AnHiAWlrYQc&ab_channel=ParamountPlus

Read sections 6.1 and 6.3 of this chapter.

https://www.sciencedirect.com/science/article/pii/B9780123749574000153#s0145

Read this section of the documentation

https://docs.zephyrproject.org/2.7.5/reference/kernel/scheduling/index.html

### Questions
What are reschedule points?

What is the difference between a preemptive and cooperative slicing?

Give an example of a hard real-time requirement. What are the consequences of missing the deadline?

Give an example of a soft real-time requirement. What are the consequences of missing the deadline?

How could you use cooperative threads to implement mutually exclusion without the need for a kernel object, such as a semaphore.
