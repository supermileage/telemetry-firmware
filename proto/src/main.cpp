#include "Particle.h"
#include "JsonMaker.h"
#include "SensorEcu.h"
#include "SensorGps.h"
#include "SensorThermo.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Publish to Cloud (Disable when not necessary during dev to save data)
#define PUBLISH_ENABLED             1
// Output Serial messages (disable for production)
#define OUTPUT_SERIAL_MSG           1
// Log and output delay for each sensor poll and new message
#define LOG_TIMING                  0

#if OUTPUT_SERIAL_MSG
    #define DEBUG_SERIAL(x) Serial.println(x)
#else
    #define DEBUG_SERIAL(x)
#endif

#define PUBLISH_INTERVAL_MS         5000
#define GPS_UPDATE_INTERVAL_MS      1000
#define THERMO_UPDATE_INTERVAL_MS   500

JsonMaker jsonMaker;
SensorEcu ecu(&Serial1);
SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermoA(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);

uint32_t lastPublish = 0;

// Timing logs
long start;
long time_ecu_poll = 0;
long time_gps_poll = 0;
long time_thermo_poll = 0;
long time_build_msg;

/**
 * Publishes a new message to Particle Cloud
 * */
void publishMessage() {
    if(LOG_TIMING) start = micros();

    // Create JSON object for publish
    jsonMaker.refresh();
    // ECU data
    jsonMaker.add("PROTO-ECT", ecu.getECT());
    jsonMaker.add("PROTO-IAT", ecu.getIAT());
    jsonMaker.add("PROTO-RPM", ecu.getRPM());
    jsonMaker.add("PROTO-UBADC", ecu.getUbAdc());
    jsonMaker.add("PROTO-O2S", ecu.getO2S());
    jsonMaker.add("PROTO-SPARK", ecu.getSpark());
    // GPS data
    jsonMaker.add("PROTO-Location", gps.getSentence());
    jsonMaker.add("PROTO-Speed", gps.getSpeedKph());

    if(LOG_TIMING) time_build_msg = micros() - start;

    if(PUBLISH_ENABLED){
        // Publish to Particle Cloud
        Particle.publish("Proto", jsonMaker.get(), PRIVATE, WITH_ACK);
        DEBUG_SERIAL("Publish - ENABLED - Message: ");
    }else{
        DEBUG_SERIAL("Publish - DISABLED - Message: ");
    }
    DEBUG_SERIAL("New JSON Message: " + jsonMaker.get());

    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("Not in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermoA.getTemp()) + "C");
    DEBUG_SERIAL();
    
    // If log timing is enabled, output time for delay at every publish 
    if(LOG_TIMING) {
        DEBUG_SERIAL("TIME ELAPSED FOR: ");
        DEBUG_SERIAL("ECU Polling:        " + String(time_ecu_poll) + "us");
        DEBUG_SERIAL("GPS Polling:        " + String(time_gps_poll) + "us");
        DEBUG_SERIAL("Thermo Polling:     " + String(time_thermo_poll) + "us");
        DEBUG_SERIAL("Build JSON Message: " + String(time_build_msg) + "us");
        time_ecu_poll = 0;
        time_gps_poll = 0;
        time_thermo_poll = 0;
    }
}

/**
 * 
 * SETUP
 * 
 * */
void setup() {
    Serial.begin(115200);

    ecu.begin();
    gps.begin();
    thermoA.begin();

    DEBUG_SERIAL("TELEMETRY ONLINE");
}

/**
 * 
 * LOOP
 * 
 * */
void loop() {
    if(LOG_TIMING) start = micros();

    ecu.handle();
    if(LOG_TIMING) {
        long time_elapsed = micros() - start;
        if(time_elapsed > time_ecu_poll) time_ecu_poll = time_elapsed;
        start = micros();
    }

    gps.handle();
    if(LOG_TIMING) {
        long time_elapsed = micros() - start;
        if(time_elapsed > time_gps_poll) time_gps_poll = time_elapsed;
        start = micros();
    }

    thermoA.handle();
    if(LOG_TIMING) {
        long time_elapsed = micros() - start;
        if(time_elapsed > time_thermo_poll) time_thermo_poll = time_elapsed;
    }

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        lastPublish = millis();
        publishMessage();
    }
}



