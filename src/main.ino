/**
 * Preliminary support for arbitrary CAN support on the Particle IO, 
 * for the Supermileage telemetry project. 
 * 
 *
 */


/**
 * Creates a new CANChannel object to connect to a CAN bus on 
 * specified pins. It takes 3 parameters.
 * CANChannel can(pins, rxQueueSize, txQueueSize)
 * @param pins CAN_D1_D2 for the Electron
 * @param rxQueueSize the queue size for received messages, default 32
 * @param txQueueSize the queue size for transmitted messages, default 32
 */
CANChannel can(CAN_D1_D2);

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
  can.begin(12500);
  Serial.begin(9600); // for testing purposes, we'll also send to serial

}

void loop() {
  if (can.errorStatus() == CAN_BUS_OFF) {
    // Causes are probably wrong baud rate or the Particle is alone
    Serial.println("CAN Bus error");
  }

}