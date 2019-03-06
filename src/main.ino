#include "can-telemetry.h"
#include "Particle-GPS.h"
#include <math.h>

/**
 * Preliminary support for arbitrary CAN support on the Particle IO, 
 * for the Supermileage telemetry project.
 */
const int GPS_SERIAL_BAUD = 9600;
const int CAN_BAUD_RATE   = 1000000;
const unsigned long RATE  = 2000; // in milliseconds. note that it should probably be 
                                  // > 2000 ms because the particle publishes at ~1 event/s
const int TIMEOUT_T   = 500;
const int NODE_ID     = 1;

// Allow polling before data works
SYSTEM_THREAD(ENABLED);

CANChannel can(CAN_D1_D2);
CANTelemetry tele(can, NODE_ID);
// Time
unsigned long time_ms;

// Create the GPS instance using TX/RX on the Electron
Gps gps = Gps(&Serial1);

// Set a timer to fire callback every millisecond
Timer timer = Timer(1, onSerialData);
CANMessage msg;

// Extra helper global variables
float ff = 0;
float x = 0;

void setup() {
  // Init the GPS
  gps.begin(GPS_SERIAL_BAUD);
  // Request all data
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  // Start the timer
  timer.start();
  // Set up serial output (probably can disable)
  Serial.begin(GPS_SERIAL_BAUD);
  // Begin CAN bus
  can.begin(CAN_BAUD_RATE);
  // Initialize the telemetry library
  tele.init();
  // Mark the time of completion
  time_ms = millis();
}


void loop() {
  unsigned long curr_time_ms = millis();
  if (curr_time_ms - time_ms >= RATE) { // Fire events every time we desire
    Serial.println("Battery");
    Serial.print("expected: ");
    x = sin(ff);
    ff = ff + 0.4;
    Serial.println(x);
    unsigned char * p = reinterpret_cast<unsigned char *>(&x);
    tele.adjust(msg, 300, false, 8, {*(p + 3), *(p + 2), *(p + 1), *p});
    uint64_t battery_emu = tele.poll(400, msg);
    float x_resp = tele.interpret<float>(battery_emu, 3, 0);
    int errors = tele.interpret<int>(battery_emu, 4, 7);
    Serial.print("response: ");
    Serial.println(x_resp);
    Serial.print("tderrors: ");
    Serial.println(errors);
    // Serial.println("Throttle");
    // tele.change_timeout(500);
    // uint64_t throttle = tele.poll(1);
    // tele.change_timeout(1000);
    if (Particle.connected()) {
      if (!isnan(x_resp + 1)) {
        Serial.println("Publishing power");
        Particle.publish("Power", String(x_resp + 1), PUBLIC, WITH_ACK);
        Serial.println("Published power");
      } else {
        Serial.println("Power is not a number");
      }
      if (true) { // TODO velocity value !isnan("10")
        Serial.println("Publishing velocity");
        Particle.publish("Velocity", String(random(10)), PUBLIC, WITH_ACK);
        Serial.println("Published velocity");
      } else {
        Serial.println("Velocity is not a number");
      }
      if (getGPRMCGPSString() != NULL) {
        Serial.println("Publishing location");
        Serial.println(*getGPRMCGPSString());
        Particle.publish("Location", *getGPRMCGPSString(), PUBLIC, WITH_ACK);
        Serial.println("Location published");
      } else {
        Serial.println("GPS not locked");
      }
      // This is a future object, testing it will cause it to block
    }
    time_ms = millis();
  }
}


void onSerialData() {
  gps.onSerialData();
}

String * getGPRMCGPSString() {
  // Data is okay
  // Is this always deterministic? It should be
  if (gps.data[4].charAt(18) == 'A') {
    return &gps.data[4];
  } else {
    return NULL;
  }
}