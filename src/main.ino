#include "math.h"
#include "can-telemetry.h"
/**
 * Preliminary support for arbitrary CAN support on the Particle IO, 
 * for the Supermileage telemetry project. 
 * 
 *
 */
const bool DEBUG          = true;
const int BAUD_RATE       = 50000;
const unsigned long RATE  = 1000; // in milliseconds. note that it should probably be 
                                  // > 2000 ms because the particle publishes at ~1 event/s
const int TIMEOUT_T   = 1000;
const int NODE_ID     = 1;
const int CMD_ID      = 0x009; // The request cmd ID on the ODrive

// SYSTEM_THREAD(ENABLED); // Non-blocking threading for Particle.publish()

/**
 * Creates a new CANChannel object to connect to a CAN bus on 
 * specified pins. It takes 3 parameters.
 * CANChannel can(pins, rxQueueSize, txQueueSize)
 * @param pins CAN_D1_D2 for the Electron
 * @param rxQueueSize the queue size for received messages, default 32
 * @param txQueueSize the queue size for transmitted messages, default 32
 */
CANChannel can(CAN_D1_D2); // set a bigger queue just in case. TODO test this
CANTelemetry tele(can, 50000, true);

// Time 
unsigned long time_ms;

// Our message ID. It should probably be a constant
uint32_t command = (NODE_ID << 5) + CMD_ID;

/**
 * For reference, the CANMessage object (sent and received by 
 * our CANChannel) is as follows
struct CANMessage
{
   uint32_t id;
   bool     extended;
   bool     rtr;
   uint8_t  len;
   uint8_t  data[8];
}
*/

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
      Particle.publish("Payload", String(x));
    }
  }
}

/**
 * Take an array of bytes and rebuild it as a float. 
 * Expects little-endian.
 */
float decode_message(uint8_t *arr, int start, int end) {
  uint8_t n[4];
  for (int i = start, a = 0; i <= end; i++, a++) {
    n[a] = arr[i];
  }
  return *(float *)n;
}