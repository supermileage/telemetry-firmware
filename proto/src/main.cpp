#include "Telemetry.h"
#include "SensorEcu.h"

SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermoA(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorEcu ecu(&Serial1);

Sensor *sensors[3] = {&ecu, &gps, &thermoA};

DataQueue dataQ;

uint32_t lastPublish = 0;

/**
 * Publishes a new message to Particle Cloud
 * */
void generateMessage() {
    long start, json_build_time;
    if (LOG_TIMING) {
        start = micros();
    }

    dataQ.resetData();

    // ECU data
    dataQ.add("PROTO-ECT", ecu.getECT());
    dataQ.add("PROTO-IAT", ecu.getIAT());
    dataQ.add("PROTO-RPM", ecu.getRPM());
    dataQ.add("PROTO-UBADC", ecu.getUbAdc());
    dataQ.add("PROTO-O2S", ecu.getO2S());
    dataQ.add("PROTO-SPARK", ecu.getSpark());
    // GPS data
    dataQ.add("PROTO-Location", gps.getSentence());
    dataQ.add("PROTO-Speed", gps.getSpeedKph());

    if (LOG_TIMING) {
        json_build_time = micros() - start;
    }

    DEBUG_SERIAL("------------------------");
    if(PUBLISH_ENABLED){
        DEBUG_SERIAL("Publish - ENABLED - Message: ");
        // Publish to Particle Cloud
        DEBUG_SERIAL(dataQ.publish("Proto", PRIVATE, WITH_ACK));
    }else{
        DEBUG_SERIAL("Publish - DISABLED - Message: ");
        DEBUG_SERIAL(dataQ.resetData());
    }

    if(OUTPUT_FREE_MEM){
        DEBUG_SERIAL("\nFree RAM: " + String(System.freeMemory()) + "B / 80000B");
    }


    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("\nNot in Message: ");
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

    Time.zone(TIME_ZONE);

    for (Sensor *s : sensors) {
        s->begin();
    }

    DEBUG_SERIAL("TELEMETRY ONLINE - PROTO");
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
    if(!Time.isValid()){
        if(gps.getTimeValid()){
            Time.setTime(gps.getTime());
        }
    }

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        lastPublish = millis();
        generateMessage();
    }
}



