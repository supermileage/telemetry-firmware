#include "Particle.h"
#include "JsonMaker.h"
#include "Sensor.h"
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

Sensor *sensors[3] = {&ecu, &gps, &thermoA};

uint32_t lastPublish = 0;

/**
 * Publishes a new message to Particle Cloud
 * */
void publishMessage() {
    long start, json_build_time;
    if (LOG_TIMING) {
        start = micros();
    }

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

    if (LOG_TIMING) {
        json_build_time = micros() - start;
    }

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
    if (LOG_TIMING) {
        DEBUG_SERIAL("Build JSON Message: " + String(json_build_time) + "us");
    }
}

/**
 * 
 * SETUP
 * 
 * */
void setup() {
    Serial.begin(115200);

    for (Sensor *s : sensors) {
        s->begin();
    }

    DEBUG_SERIAL("TELEMETRY ONLINE");
}

/**
 * 
 * LOOP
 * 
 * */
void loop() {
    for (Sensor *s : sensors) {
        if (LOG_TIMING) {
            long time = s->benchmarkedHandle();
            DEBUG_SERIAL(s->getHumanName() + " polling: " + String(time) + "us");
        } else {
            s->handle();
        }
    }

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        lastPublish = millis();
        publishMessage();
    }
}



