/**
 * Preliminary support for arbitrary CAN support on the Particle IO, 
 * for the Supermileage telemetry project. 
 * 
 *
 */
const bool DEBUG          = true;
const int BAUD_RATE       = 50000;
const unsigned long RATE  = 2000; // in milliseconds. note that it should probably be 
                                  // > 2000 ms because the particle publishes at ~1 event/s
const uint8_t NODE_ID     = 1;
const uint8_t CMD_ID      = 0x009; // The request cmd ID on the ODrive

SYSTEM_THREAD(ENABLED); // Non-blocking threading for Particle.publish()

/**
 * Creates a new CANChannel object to connect to a CAN bus on 
 * specified pins. It takes 3 parameters.
 * CANChannel can(pins, rxQueueSize, txQueueSize)
 * @param pins CAN_D1_D2 for the Electron
 * @param rxQueueSize the queue size for received messages, default 32
 * @param txQueueSize the queue size for transmitted messages, default 32
 */
CANChannel can(CAN_D1_D2, 64); // set a bigger queue just in case. TODO test this

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
  if (DEBUG) {
    can.begin(BAUD_RATE, CAN_TEST_MODE);
    Serial.begin(9600);
  } else {
    can.begin(BAUD_RATE);
  }
  can.addFilter(command, 0x7FF); // We're only listening for this specific thing
}

void loop() {
  unsigned long curr_time_ms = millis();
  if (curr_time_ms - time_ms >= RATE) { // Fire events every time we desire
  time_ms = curr_time_ms; // Timestamp
    if (can.errorStatus() == CAN_BUS_OFF) {
      if (DEBUG) Serial.println("CAN bus error");
    } else {
      // Send message phase
      CANMessage message;
      message.id = command;
      if (DEBUG) {
        message.len = 1;
        message.data[0] = 0xFF;
      } else {
        message.len = 0; // send with no payload
      }
      can.transmit(message);

      // Wait for a response 
      CANMessage response;
      uint64_t response_msg = 0;
      // Blocking, but it shouldn't take longer than our RATE
      while (can.receive(response)) { // TODO Are we always guaranteed one message in queue? 
        response_msg = decode_message(response.data, response.len);
        if (DEBUG) {
          Serial.println("Available serial data " + String(can.available()));
          Serial.println("Response ID " + response.id);
          Serial.println("Response msg " + int(response_msg));
        }
      }

      // Relay to electron
      if (Particle.connected()) {
        // TODO grab values properly, probably
        Particle.publish("Payload", String(int(response_msg)), PRIVATE, NO_ACK);
      }
    }
  }
}

/**
 * Returned value is in little-endian, thus we must go backwards
 * TODO probably split it up so it takes a start and end index
 */
uint64_t decode_message(uint8_t *arr, int len) {
  uint64_t num = 0;
  for (int i = 0; i < len; i++) {
    num += uint64_t(arr[i]) << (8 * i); // must cast to a bigger byte
  }
  return num;
}