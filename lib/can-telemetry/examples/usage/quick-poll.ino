// Example usage for can-telemetry library by Leslie Cheng.

#include "can-telemetry.h"

// Initialize a CAN network 
CANChannel can(CAN_D1_D2);

// Initialize the telemetry object, node id 0
CANTelemetry tele(can, 0);

unsigned long time_ms;

void setup() {
    // No real setup required
    can.begin(50000); // Begin with baud rate 50000
    tele.init();      // Initialize our telemetry
    Serial.begin(9600);
}

void loop() {
    // Build a CANMessage
    CANMessage msg = tele.createMsg(0x054, true, 8, {0, 0, 0});
    uint64_t val = tele.poll(0x054, msg); // Filter for 0x054
    // Interpret the bytes as a float
    float ret_float = tele.interpret<float>(val, 0, 3);
    // Interpret the bytes as a float, big-endian
    float ret_float_be = tele.interpret<float>(val, 3, 0);
    // Interpret the bytes as an int
    int ret_int = tele.interpret<int>(val, 0, 3);
    // Wait 5 seconds
    delay(5000);
}
