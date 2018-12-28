## Notes about CAN 
CAN requires us to acknowledge all messages or the node that's sending will retransmit unacknowledged frames over and over. So in order to account for buffer overflow, we should take steps to acknowledge all the telemetry data. It looks like the library does this automatically for us. 
But also an important caveat: all nodes can listen in on the CAN bus. 

## Notes about ODrive
We'll need to install [this](https://github.com/Wetmelon/ODrive/tree/feature/CAN) beta version of the ODrive control software, because the release version doesn't have support for CAN just yet. 

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