#include "math.h"
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

float x = 0;
float y = sin(x) * 10;

void loop() {
  unsigned long curr_time_ms = millis();
  if (curr_time_ms - time_ms >= RATE) { // Fire events every time we desire
    time_ms = curr_time_ms; // Timestamp
    if (can.errorStatus() == CAN_BUS_OFF) {
      if (DEBUG) Serial.println("CAN_BUS_OFF set");
    } else {
      can.begin(BAUD_RATE);
      can.addFilter(command, 0x7FF); // We're only listening for this specific thing
      if (DEBUG) Serial.println("Sending message");
      // Send message phase
      CANMessage message;
      message.id = command;
      message.len = 8;
      // memcpy(message.data, &x, sizeof(float));
      uint8_t * n = reinterpret_cast<uint8_t *>(&x);
      message.data[0] = *n; //int32_t(x);
      message.data[1] = *(n + 1); //int32_t(x) >> 8;
      message.data[2] = *(n + 2); //int32_t(x) >> 16;
      message.data[3] = *(n + 3); //int32_t(x) >> 24;

      // memcpy(&message.data[4], &y, sizeof(float));
      n = reinterpret_cast<uint8_t *>(&y);
      message.data[4] = *n;
      message.data[5] = *(n + 1); //int32_t(y) >> 8;
      message.data[6] = *(n + 2); //int32_t(y) >> 16;
      message.data[7] = *(n + 3); //int32_t(y) >> 24;

      // Probably a good idea to do some error checking
      if (can.transmit(message)) {
        Serial.println("Sent.");
      }
      // Error status is only set on transmit
      if (can.errorStatus() == CAN_NO_ERROR) {
        // Wait for a response 
        CANMessage response;
        float response_x = 0;
        float response_y = 0;

        // Blocking, but it shouldn't take longer than our RATE
        bool received = false;
        bool timeout = false;
        while (!received && !timeout) {
          if (can.receive(response)) { // TODO Are we always guaranteed one message in queue? 
            response_x = decode_message(response.data, 0, 3);
            response_y = decode_message(response.data, 4, 7);
            // memcpy(&response_x, response.data, sizeof(float));
            // memcpy(&response_y, &response.data[4], sizeof(float));
            if (DEBUG) {
              Serial.print("Expected x: ");
              Serial.println(x);
              Serial.print("Expected y: ");
              Serial.println(y);
              Serial.print("Response ID: ");
              Serial.println(response.id);
              Serial.print("Response x: ");
              Serial.println(response_x);
              Serial.print("Response y: ");
              Serial.println(response_y);
            }
            received = true;
          }
          if (millis() - time_ms >= TIMEOUT_T) timeout = true;
        }
        can.end(); // kill the bus
        // Relay to electron
        if (Particle.connected()) {
          // TODO grab values properly, probably
          Particle.publish("Payload", String(response_x) + " " + String(response_y), PUBLIC, NO_ACK);
        }

        x += 0.1;
        y = sin(float(x)) * 10;
      } else {
        if (DEBUG) Serial.println("CAN error");
      }
      
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