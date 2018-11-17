## Notes about CAN 
CAN requires us to acknowledge all messages or the node that's sending will retransmit unacknowledged frames over and over. So in order to account for buffer overflow, we should take steps to acknowledge all the telemetry data. It looks like the library does this automatically for us. 
But also an important caveat: all nodes can listen in on the CAN bus. 