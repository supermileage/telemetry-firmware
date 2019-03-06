# can-telemetry

A Particle library that allows for basic telemetry through the CAN network. 

## Get started 

To get started, either clone this repo and add it as a private library in your Particle account, or add the files to your source directory. 

For the first option, run the commands `particle library upload`, and add it to a project by running `particle library add can-telemetry`.

## Usage

Add the library as a dependency to your particle project (`particle library add can-telemetry`), or add the source files directly to your project source. Then make sure you include the header `#include "can-telemetry.h"`

See the [examples](examples) folder for more details.

## Documentation

`CANTelemetry(CANChannel &channel, int node_id, int timeout)`
- `channel` - the CAN channel object
- `node_id` - the ID of this telemetry module on the bus
- `timeout` - the desired timeout before we stop listening for a response (optional, default 1000 ms)

`init()`
- Sets a mask to filter out all messages. Call this immediately following `begin()` on the CAN object.

`poll(uint32_t filter, CANMessage msg)`
- `filter` - the response ID desired
- `msg` - CANMessage to broadcast onto the network. Essentially, it is our query packet. (Optional)

If a `msg` is not specified, the polling method will default to passive polling (useful for if there's already a constant stream of a particular message)

`void adjust(CANMessage &msg, uint32_t id, bool rtr, uint8_t len, std::initializer_list<uint8_t> nums)`
- `msg` - CANMessage to modify
- `id` - ID to change the msg to 
- `rtr` - RTR bit option
- `len` - length of the data (optional, default 0)
- `nums` - initializer list of the data (optional, default empty)

Note on `initializer_list`: it allows you to pass in parameters using curly braces, without declaring it elsewhere. Therefore, `adjust(msg, 5, true, 8, {0, 1, 2, 3, 4, 5, 6, 7});` is perfectly valid.

`heartbeat()`
- Sends a heartbeat packet to the bus and returns 1 if no error is returned. 0 otherwise. 

`change_timeout(int timeout)`
- `timeout` - timeout to change to (optional, default 1000 ms)

`interpret<type>(uint64_t data, int start, int end)`
- `type` - the desired return type
- `data` - the 8 byte data to interpret
- `start` - the starting byte
- `end` - the ending byte
NOTE: if start > end, `interpret` assumes big-endian.