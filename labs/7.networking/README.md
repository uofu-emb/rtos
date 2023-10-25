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
Add the following configuration to prj.conf

```
CONFIG_CAN=y
CONFIG_CAN_INIT_PRIORITY=80
CONFIG_CAN_MAX_FILTER=5
```
Note that the CAN device is "CAN_1" on the discovery board.

## Activity 0
This activity will establish sending and receiving messages.

1. Set the CAN device to loopback mode with `can_set_mode`
1. Create a function that sends a message periodically using `can_send`.
1. Register a callback with `can_attach_isr` to receive the messages
    1. Set the filter to receive any messages.

Hint: follow the examples in the documentation.

## Activity 1
This activity will deal with filtering for specific message types.

1. Create a function that sends two different types of messages.
1. Register a callback which filters only one type of message.

## Activity 2
This activity will work with the CAN transceiver and connecting multiple boards.

1. Wire up CAN transceiver to your boards.
    1. Remember to disable loopback mode.
    1. RX is on PB8 and TX is on PB9.
1. Run the code from activity 1 and observe the electrical behavior of the bus line and the microcontroller lines.

## Activity 3
This activity will observe bus contention from a babbling node. "In a Controller Area Network a babbling node is a node continuously (and usually erroneously) transmitting CAN frames with identical - often high - priority."

1. Setup a receiver using a worker queue.
1. On one board, broadcast messages with a high priority continously.
1. On the other board, broadcast a message with a lower priority periodically.
1. use oscilloscope to observe the access pattern.
1. Observe how often the lower priority message is actually transmitted and received.
1. Introduce a short busy wait delay into the babbling node, and slowly increase the duration until you observe the other messages consistently
