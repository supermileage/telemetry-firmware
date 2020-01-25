#include "can-telemetry.h"
#include "Particle-GPS.h"
#include "SparkFunMAX31855k.h"
#include <math.h>

const int GPS_SERIAL_BAUD = 9600;
const int CAN_BAUD_RATE   = 500000;
const unsigned long RATE  = 5000; // in milliseconds. note that it should probably be 
                                  // > 2000 ms because the particle publishes at ~1 event/s
const int TIMEOUT_T   = 500;
const int NODE_ID     = 1;

const int TEMP_1 = D5;
const int TEMP_2 = D6;

// Allow polling before data works
SYSTEM_THREAD(ENABLED);

// CANChannel can(CAN_D1_D2);
// CANTelemetry tele(can, NODE_ID, TIMEOUT_T);
// Time
unsigned long time_ms;

// Create the GPS instance using TX/RX on the Electron
Gps gps(&Serial1);

// Set a timer to fire callback every millisecond
Timer timer(1, onSerialData);

// Thermocouple stuff, B1 and B2 are arbitrary, we don't need it
// SparkFunMAX31855k thermo1(&SPI, TEMP_1);
// SparkFunMAX31855k thermo2(&SPI, TEMP_2);


void setup() {
  // SPI.begin();
  // Init the GPS
  gps.begin(GPS_SERIAL_BAUD);
  // Request all data
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  // Start the timer
  timer.start();
  // Set up serial output (probably can disable)
  Serial.begin(GPS_SERIAL_BAUD);
  // Begin CAN bus
  // can.begin(CAN_BAUD_RATE);
  // Initialize the telemetry library
  // tele.init();
  // Mark the time of completion
  time_ms = millis();
}

float x = 0.0;

float y = sin(x);


void loop() {
  unsigned long curr_time_ms = millis();

  if (curr_time_ms - time_ms >= RATE) {
    // Serial.println("Battery");
    // // Battery stuff
    // // Send a message with header 0x201, first byte 20
    // uint64_t batt_soc = tele.poll(0x241, tele.createMsg(0x201, false, 8, {0x1A, 0, 0, 0, 0, 0, 0, 0}));
    // int batt_soc_resp = 0;
    // if (tele.interpret<int>(batt_soc, 0, 0) == 0x01) {
    //   batt_soc_resp = tele.interpret<int>(batt_soc, 2, 5);
    //   Serial.print("Battery SOC: ");
    //   Serial.println(batt_soc_resp);
    // }
    // uint64_t batt_onl = tele.poll(0x241, tele.createMsg(0x201, false, 8, {0x18, 0, 0, 0, 0, 0, 0, 0}));
    // int batt_onl_resp = tele.interpret<int>(batt_onl, 2, 3);
    // Serial.print("Online status: ");
    // Serial.println(batt_onl_resp);
    initGPS();
    // // GPS stuff
    String gprmc = getGPRMCGPSString();
    String gpvtc = getGPVTGGPSString();

    printNMEAStrings();

    // if (gps != NULL) {
    //   Serial.println("GPS data fine");
    // }

    // // Temperature stuff
    // float temp1 = thermo1.readTempC();
    // float temp2 = thermo2.readTempC();
    // if (!isnan(temp1)) {
    //   Serial.print("Temperature1 ");
    //   Serial.println(temp1);
    // }
    // if (!isnan(temp2)) {
    //   Serial.print("Temperature2 ");
    //   Serial.println(temp2);
    // }
    // x += 0.2;
    // y = sin(x);

    // IoT stuff
    if (Particle.connected()) {

      if (gprmc != "") {
        Serial.println("Publishing location");
        Particle.publish("PROTO-Location", gprmc, PUBLIC, WITH_ACK);
      }
      
      if (gpvtc != "") {
        Serial.println("Publishing velocity");
        Particle.publish("PROTO-Speed", gpvtc, PUBLIC, WITH_ACK);
      }
      // if (tele.interpret<int>(batt_soc, 0, 0) == 0x01 
      //     && tele.interpret<int>(batt_onl, 0, 0) == 0x01) { 
        // Serial.println("Publishing SOC/Onl value");
        // String soc = String(y);
        // String onl = String(y);
        // String batt_payload = soc + " " + onl;
        // Particle.publish("Power", soc, PUBLIC, WITH_ACK);
        // Serial.println("Published SOC/Onl value");
      // } else {
        // Serial.println("Invalid SOC or onl value");
      // }
      // if (!isnan(temp1) && !isnan(temp2)) {
      //   Serial.println("Publishing temperature values");
      //   String t1 = String(temp1);
      //   String t2 = String(temp2);
      //   String t_tot = t1 + " " + t2;
        // Particle.publish("Temperature", String(y), PUBLIC, WITH_ACK);
        // Serial.println("Published temp values");
    //   } else {
    //     Serial.println("Invalid temp values");
    //   }
    //   if (gps != NULL) {
    //     Serial.println("Publishing location");
    //     Particle.publish("Location", *gps, PUBLIC, WITH_ACK);
    //     Serial.println("Location published");
    //   } else {
    //     Serial.println("GPS not locked");
    //   }
    // }
    time_ms = millis();
  }
  }
}

void onSerialData() {
  gps.onSerialData();
}

void initGPS() {
  gps.sendCommand(PMTK_SET_BAUD_9600);
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ);
  gps.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
  gps.sendCommand(PMTK_ENABLE_WAAS);
  gps.sendCommand(PGCMD_ANTENNA);
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
}

void printNMEAStrings() {
  for (int i = 0; i < 7; i++) {
    Serial.println(gps.data[i]);
  }
}

String getGPRMCGPSString() {
  if (gps.data[4].charAt(18) == 'A') {
    return String(gps.data[4]);
  } else {
    return "";
  }
}

String getGPVTGGPSString() {
  if (gps.data[4].charAt(18) == 'A') {
    return String(gps.data[5]);
  } else {
    return "";
  }
}