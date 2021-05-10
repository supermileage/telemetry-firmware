#include "Telemetry.h"
#include "SensorEcu.h"

JsonMaker jsonMaker;

SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermoA(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorEcu ecu(&Serial1);

Sensor *sensors[3] = {&ecu, &gps, &thermoA};

uint32_t lastPublish = 0;

/**
 * Publishes a new message to Particle Cloud
 * */
void generateMessage() {
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

    String jsonString = jsonMaker.get();
    publishMessage("Proto", jsonString);

    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("Not in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermoA.getTemp()) + "C");
    DEBUG_SERIAL();
    
    // If log timing is enabled, output time for delay at every publish 
    if (LOG_TIMING) {
        DEBUG_SERIAL("Build JSON Message: " + String(json_build_time) + "us");
        for (Sensor *s : sensors) {
            DEBUG_SERIAL(s->getHumanName() + " polling: " + String(s->getLongestHandleTime()) + "us");
        }
        DEBUG_SERIAL();
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
            s->benchmarkedHandle();
        } else {
            s->handle();
        }
    }

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        lastPublish = millis();
        generateMessage();
    }
}



