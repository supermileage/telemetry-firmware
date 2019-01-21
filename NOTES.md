## Notes about CAN 
CAN requires us to acknowledge all messages or the node that's sending will retransmit unacknowledged frames over and over. So in order to account for buffer overflow, we should take steps to acknowledge all the telemetry data. It looks like the library does this automatically for us. 
But also an important caveat: all nodes can listen in on the CAN bus. 

## Notes about ODrive
We'll need to install [this](https://github.com/Wetmelon/ODrive/tree/feature/CAN) beta version of the ODrive control software, because the release version doesn't have support for CAN just yet. 

In particular, we need to grab the encoder_pos_estimate (counts) and encoder_vel_estimate (counts/s). 

---

The CAN code we're (probably) interested in wrt the ODrive is


```
void CANSimple::get_encoder_estimates_callback(Axis* axis, CAN_message_t& msg) {
    CAN_message_t txmsg;
    txmsg.id = axis->config_.can_node_id << NUM_CMD_ID_BITS;
    txmsg.id += MSG_GET_ENCODER_ESTIMATES;  // heartbeat ID
    txmsg.isExt = false;
    txmsg.len = 8;

    // Undefined behaviour!
    // uint32_t floatBytes = *(reinterpret_cast<int32_t*>(&(axis->encoder_.pos_estimate_)));

    uint32_t floatBytes;
    static_assert(sizeof axis->encoder_.pos_estimate_ == sizeof floatBytes);
    std::memcpy(&floatBytes, &axis->encoder_.pos_estimate_, sizeof floatBytes);
    
    txmsg.buf[0] = floatBytes;
    txmsg.buf[1] = floatBytes >> 8;
    txmsg.buf[2] = floatBytes >> 16;
    txmsg.buf[3] = floatBytes >> 24;

    static_assert(sizeof floatBytes == sizeof axis->encoder_.vel_estimate_);
    std::memcpy(&floatBytes, &axis->encoder_.vel_estimate_, sizeof floatBytes);
    txmsg.buf[4] = floatBytes;
    txmsg.buf[5] = floatBytes >> 8;
    txmsg.buf[6] = floatBytes >> 16;
    txmsg.buf[7] = floatBytes >> 24;

    odCAN->write(txmsg);
}
```
And the txmsg is defined as 
```
typedef struct {
    uint32_t id = 0x000;  // 11-bit max is 0x7ff, 29-bit max is 0x1FFFFFFF
    bool isExt = false;
    uint8_t len = 8;
    uint8_t buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
} CAN_message_t;
```

Note that the byte order is little-endian.

So the end goal is to send a CAN frame with the ID as whatever we set the drives to be, followed by the command (0x009 in this case).

If bit-banging is necessary, from what I can tell we can just put together a message that has the first 6 bits as the node ID, next 5 as the command ID, then just set the rest of the frame to 0. It should then respond with the same ID bits, and a payload (as detailed above).

Reference: [interface_can.cpp](https://github.com/Wetmelon/ODrive/blob/feature/CAN/Firmware/communication/interface_can.cpp), [interface_can.hpp](https://github.com/Wetmelon/ODrive/blob/feature/CAN/Firmware/communication/interface_can.hpp), [can_simple.cpp](https://github.com/Wetmelon/ODrive/blob/feature/CAN/Firmware/communication/can_simple.cpp), [can_simple.hpp](https://github.com/Wetmelon/ODrive/blob/feature/CAN/Firmware/communication/can_simple.hpp).

# Baud Rate 

We can change the baud rate, but the ODrive allows us to configure the baud rate with `<odrv>.can.set_baud_rate(<value>)`. Don't forget to call `<odrv>.save_configuration()` so it doesn't reset every reboot.

Example is 
```
odrv0.axis0.config.can_node_id = 3
odrv0.axis1.config.can_node_id = 1
odrv0.can.set_baud_rate(500000)
odrv0.save_configuration()
odrv0.reboot()
```

# Non-blocking `Particle.publish()`

There are a couple caveats regarding the `Particle.publish()` functionality, even if `SYSTEM_THREAD(ENABLED)` is set. It looks like if the connection is lossy or completely disconnected, the function will block and basically kill the Electron. There are a couple work-arounds, like checking `Particle.connected()` before firing a publish, or ignoring ACK (though I'm not convinced this actually makes a big difference). 

Another option is to use [this library](https://github.com/rickkas7/PublishQueueAsyncRK), which seems promising. Will investigate. 

# MBed compilation 

Set up the environment:

`mbed new .`

Fix the includePaths:

`mbed export -i vscode_gcc_arm -m lpc1768`

Compile with:

`mbed compile -t GCC_ARM -m lpc1768`

Optionally, add `--flash` at the end in order to flash it to the board.

---
### Signals
Name | Type | Bits | Factor | Offset | Byte Order
:-- | :-- | :--: | --: | :--: | :--:
Axis Error | Unsigned Int | 32 | 1 | 0 | Intel
Axis Current State | Unsigned Int | 32 | 1 | 0 | Intel
Motor Error | Unsigned Int | 32 | 1 | 0 | Intel
Encoder Error | Unsigned Int | 32 | 1 | 0 | Intel
Sensorless Error | Unsigned Int | 32 | 1 | 0 | Intel
Axis CAN Node ID | Unsigned Int | 16 | 1 | 0 | Intel
Axis Requested State | Unsigned Int | 32 | 1 | 0 | Intel
Encoder Pos Estimate | IEEE 754 Float | 32 | 1 | 0 | Intel
Encoder Vel Estimate | IEEE 754 Float | 32 | 1 | 0 | Intel
Encoder Shadow Count | Signed Int | 32 | 1 | 0 | Intel
Encoder Count In CPR | Signed Int | 32 | 1 | 0 | Intel
Goal Position | Signed Int | 32 | 1 | 0 | Intel
Pos Setpoint | Signed Int | 32 | 1 | 0 | Intel
Vel FF | Signed Int | 16 | 0.1 | 0 | Intel
Current FF | Signed Int | 16 | 0.01 | 0 | Intel
Vel Setpoint | Signed Int | 32 | 0.01 | 0 | Intel
Current Setpoint | Signed Int | 32 | 0.01 | 0 | Intel
Velocity Limit | IEEE 754 Float | 32 | 1 | 0 | Intel
Traj Vel Limit | IEEE 754 Float | 32 | 1 | 0 | Intel
Traj Accel Limit | IEEE 754 Float | 32 | 1 | 0 | Intel
Traj Decel Limit | IEEE 754 Float | 32 | 1 | 0 | Intel
Traj A per CSS | IEEE 754 Float | 32 | 1 | 0 | Intel

From [here](https://github.com/Wetmelon/ODrive/blob/feature/CAN/docs/can-protocol.md)

velocity_ff is the velocity feed-forward term, in counts/s.
current_ff is the current feed-forward term, in A.
