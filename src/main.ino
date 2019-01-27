#include "math.h"
#include "can-telemetry.h"
/**
 * Preliminary support for arbitrary CAN support on the Particle IO, 
 * for the Supermileage telemetry project.
 */
const bool DEBUG          = true;
const int BAUD_RATE       = 50000;
const unsigned long RATE  = 1000; // in milliseconds. note that it should probably be 
                                  // > 2000 ms because the particle publishes at ~1 event/s
const int TIMEOUT_T   = 1000;
const int NODE_ID     = 1;
const int CMD_ID      = 0x009; // The request cmd ID on the ODrive

CANChannel can(CAN_D1_D2);
CANTelemetry tele(can, BAUD_RATE, TIMEOUT_T, DEBUG);
// Time 
unsigned long time_ms;

// Our message ID. It should probably be a constant
uint32_t command = (NODE_ID << 5) + CMD_ID;

void setup() {
  Serial.begin(9600);
  time_ms = millis();
}

void loop() {
  unsigned long curr_time_ms = millis();
  if (curr_time_ms - time_ms >= RATE) { // Fire events every time we desire
    uint64_t val = tele.poll(command);
    float x = tele.interpret<float>(val, 0, 3);
    float y = tele.interpret<float>(val, 4, 7);

    if (Particle.connected()) {
      Particle.publish("Payload", String(x) + " " + String(y), PUBLIC, NO_ACK);
    }
  }
}