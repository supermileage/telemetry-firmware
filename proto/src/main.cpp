#include "Telemetry.h"
#include "SensorEcu.h"

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

    newPayload();

    // ECU data
    addMessage("PROTO-ECT", ecu.getECT());
    addMessage("PROTO-IAT", ecu.getIAT());
    addMessage("PROTO-RPM", ecu.getRPM());
    addMessage("PROTO-UBADC", ecu.getUbAdc());
    addMessage("PROTO-O2S", ecu.getO2S());
    addMessage("PROTO-SPARK", ecu.getSpark());
    // GPS data
    addMessage("PROTO-Location", gps.getSentence());
    addMessage("PROTO-Speed", gps.getSpeedKph());

    if (LOG_TIMING) {
        json_build_time = micros() - start;
    }

    publishMessage("Proto");

    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("Not in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermoA.getTemp()) + "C");
    DEBUG_SERIAL("Current Time (UTC): " + Time.timeStr());
    DEBUG_SERIAL("Signal Strength: " + String(Cellular.RSSI().getStrength()) + "%");
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

    // If there is valid time pulled from cellular, get time from GPS (if valid)
    if(Time.now() < 1609459201){
        if(gps.getTime() > 1609459201){
            Time.setTime(gps.getTime());
        }
    }

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        lastPublish = millis();
        generateMessage();
    }
}



