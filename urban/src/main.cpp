#include "Telemetry.h"
#include "SensorCan.h"

JsonMaker jsonMaker;

SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermoA(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorCan can(&SPI1, D5, D6);

Sensor *sensors[3] = {&gps, &thermoA, &can};

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
    // GPS data
    jsonMaker.add("URBAN-Location", gps.getSentence());
    jsonMaker.add("URBAN-Temperature", String(thermoA.getTemp()) + "C");

    if (LOG_TIMING) {
        json_build_time = micros() - start;
    }

    String jsonString = jsonMaker.get();
    publishMessage("Urban", jsonString);

    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("\nNot in Message: ");

    DEBUG_SERIAL("Current Speed: " + String(gps.getSpeedKph()) + "KM/h");

    for(int i = 0; i < can.getNumIds(); i++){
        String output = "CAN ID: 0x" + String(can.getId(i), HEX) + " - CAN Data:";
        uint8_t canDataLength = can.getDataLen(i);
        unsigned char* canData = can.getData(i);
        for(int k = 0; k < canDataLength; k++){
            output += " 0x";
            output += String(canData[k], HEX);
        }
        DEBUG_SERIAL(output);
    }
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



