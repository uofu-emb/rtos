# Lab 7. Networking
## Learning objectives:

* Use the CAN bus to share data between nodes.
* Identify the failure mode of a babbling CAN node
* Identify different network topologies
* Identify the electric characteristics of the CAN wiring
* Describe the properties of CAN that allow prioritization, error handling, and multiple bus members acting at the same time
* Identify the components of the CAN bus framing.
* Setup CAN message processing using an interrupt.
* Setup CAN message processing using a work queue.
* Send a message over the CAN bus.
* Apply the 8 fallacies of distributed computing.

# Prelab
## Readings
Review the CAN API docs.

https://docs.zephyrproject.org/2.7.5/reference/networking/can_api.html

The Wikipedia article is quite comprehensive.

https://en.wikipedia.org/wiki/CAN_bus

Read the transcript or watch this talk.

https://web.archive.org/web/20171107014323/http://blog.fogcreek.com/eight-fallacies-of-distributed-computing-tech-talk/

Read this paper.

https://www.researchgate.net/publication/322500050_Fallacies_of_Distributed_Computing_Explained

Read this article

https://www.dnsstuff.com/what-is-network-topology

# Lab

## Activity 0
send messages.
Use loopback.
Add callback.

## Activity 1
use workqueue
add filter

## Activity 2
download example code to one board.
run a babbling node
use oscilloscope to observe the access pattern.

## Activity 3
Decoding car data.
