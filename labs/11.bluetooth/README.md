# Wireless communication with Bluetooth

# Learning objectives
* Describe difference between Bluetooth classic and Bluetooth LE.
* Explain the basic principles of radio modulation and Bluetooth physical layer
* Place network technologies into the context of the OSI model.
* Identify security vulnerabilities in wireless network technologies.
* Implement a Bluetooth LE service.
* Work with library code.
* Work with and expand an existing system.

# Pre-lab

Watch this video and read the article on radio modulation basics.

https://www.taitradioacademy.com/topic/how-does-modulation-work-1-1/

Chapter 2 and sections 4.2 and 4.3

https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-121r2-upd1.pdf

Read chapter 4.

https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf

Read section 3.8

https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf

Some detail on the protocol stack that may be useful.

https://www.mathworks.com/help/bluetooth/ug/bluetooth-protocol-stack.html

# Lab

Bluetooth has two modes, classic and low energy (BLE).
Bluetooth supports many protocols and profiles over each mode.
We'll be working with LE mode, and focus on the GATT profile.

The physical layer is handled by a separate dedicated wireless controller - in our case it is an Infineon CYW43439 (it's inside the little metal box).
This is the controller stack.
1. The controller handles the radio operation in the physical layer (PHY). This layer handles the modulation of
2. The demodulated radio data is structured together into a frame in the link layer (LL).
3. The microcontroller interfaces with the wireless controller over the host controller interface (HCI).
    * The RP2040 communicates with the CYW43439 over an SPI connection.
    * The HCI provides the host data in packets for consumption.
The host stack has several layers.
4. The L2CAP layer is responsible for taking the standard BLE packet and decoding the raw data into the protocol format.
5. The protocol layer is where data begins to have a semantic meaning. In this case we are using the attribute (ATT) format, which breaks the data up into fields.
    * The attribute handle, a 16-bit identifier set by the server.
    * The type, a UUID defined by the standards group which specifies what type of measurement it is.
    * The value, a variable length data.
    * Permissions, which specify security levels and privileges.
6. The GATT layer defines a client-server architecture, and coordinates the exchange of data.
    * The server advertises a set of _characteristics_, the available data.
    * The client connects to the server requests data for a characteristic.
7. The host stack also handles security throughout the process with the security manager protocol (SMP).
8. The generic access profile (GAP) specifies how devices discover and establish connections.

Diagrams and details can be found here.
https://www.mathworks.com/help/bluetooth/ug/bluetooth-protocol-stack.html#mw_0008bc2e-0554-48d1-a30c-93daab45d3ce

## Activity - advertising

### Feature request
Our device uses a generic name when advertising. We need to distinguish ourselves from other devices.

### Background

Fork the bluetooth template repo https://github.com/uofu-emb/rtos.bt.
This contains the FreeRTOS setup and a basic GATT server implementation.
The template starts with simplified code from the pico-example and btstack repos.

https://bluekitchen-gmbh.com/btstack/#examples/examples/#gatt-server-heartbeat-counter-over-gatt

To check the whether your Bluetooth is functioning, you can use your phone or computer to sniff the service.
This app was used in the Pico docs, and is available for
[iOS](https://apps.apple.com/us/app/lightblue/id557428110)
and [Android](https://play.google.com/store/apps/details?id=com.punchthrough.lightblueexplorer&hl=en_US).
It seems to work fine on my phone, but feel free to find any other tool.

### Tasks

1. Modify the service to make it easier to identify your device. In the `gatt_counter.c` file, modify the `adv_data` array, which contains the advertisement data.
   * The advertisement data consists of a set of fields.
   * Each field begins with the number of bytes (including the type byte)
   * The field type is `BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME` 0x09
   * The remaining data is UTF8 bytes.
   * You can see more details in the [specification](https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/CSS_v11/out/en/supplement-to-the-bluetooth-core-specification/data-types-specification.html)
1. Flash the program to your microcontroller. When you connect to the serial port, it should print out a message with the device address. "BTstack up and running on 28:CD:C1:0F:1D:DC."
1. You should see your device advertised with the name you specified.

## Activity - attributes, services, and characteristics.

### Feature request
Customers can find our device with a unique name now, but it doesn't use a unique name when the device service is queried.

### Background
We have three very generic terms to disambiguate.
* Attribute is the data format used to structure metadata and the data into fields.
* Service is the endpoint to access some capability of the server.
* Characteristic is the data the service provides.

### Tasks
1. Connect to your device with your phone and look at what are the services the device provides.
    1. Note the properties of each service.
    1. Read the value from some services.
1. BTStack uses a custom domain specific language to define the services your device offers.
    1. In `include/gatt_counter.gatt` you'll find the definitions of each of the services.
    1. Many services are general purpose, such as the battery service included.
    1. When the project is compiled, a CMake macro provided by BTStack will generate a header file with C code based on the gatt file.
1. Find the `GAP_SERVICE` and modify the `GAP_DEVICE_NAME` device characteristic. The characteristic has type `GAP_DEVICE_NAME`, is read only, and includes a literal string value.
    1. Characteristics and services have a globally unique defined type and identity, set by the Bluetooth standard.
    1. Custom characteristics and services are allowed to be specified with a universally unique identifier (UUID). The counter service is an example of this.
1. Flash your controller and see if the service has changed.

## Activity - HCI packet handling.

### Feature request
We need to improve our debugging by logging whenever a client connects to our device.

### Background
BTStack uses an event loop which executes callbacks in response to activity in different layers of the stack. Multiple callback handlers can be created for each layer. Recall that the HCI is the point where packets are transferred to the host from the controller. In the code we have two handlers for HCI packet data (conveniently named `packet_handler`). One in the `gatt_counter.c` file is responsible for handling activity related to our counter. The other is in `pico_bt_example_freertos.c`, which logs the device address when Bluetooth is initialized.

### Tasks
1. In `picow_bt_example_freertos.c`, modify the packet handler to log when the number of connections has changed.
1. Hint: The different types of events and helpers to extract event data from the packet are located in `bstack_event.h`

## Activity - add a service

### Feature Request
There are currently two services, the battery service and the heartbeat. Users need to know what the current temperature of the processor is.

### Background

The template includes code to read data from the internal temperature sensor on the Pico in `temp_sense.c` with functions declarations in `temp_sense.h`.

We should implement a service which matches the semantic meaning of our measurements.
"Device Information Service" seems good.
Our service should report a temperature characteristic, and should specify the data is measured in Celsius.

### Tasks
1. Find the appropriate service, unit, and characteristic ids from the [assigned numbers](https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf?v=1732413193435)
You can use the raw id, or the corresponding constants are defined in bluetooth_gatt.h in the btstack source.
1. Update the .gatt file to include the new service definition. Specify a `READ | DYNAMIC` characteristic.
1. Rebuild and flash the project and verify that the service is being advertised properly.
1. Initialize the temperature measurement during setup.
1. In the `att_read_callback`, return the data if the handle is the temperature value handle.
    1. The data is floating point, you could convert it to centidegrees like `data = (uint16_t)(temp_measurement*100);`
    1. You could also use a union to send the bytes for the floating point number directly.

# Reference implementation

Reference implementation is located at https://github.com/uofu-emb/rtos.11
