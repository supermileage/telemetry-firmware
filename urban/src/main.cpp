#include "Particle.h"
#include "JsonMaker.h"
#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorCan.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Publish to Cloud (Disable when not necessary during dev to save data)
#define PUBLISH_ENABLED             0
// Output Serial messages (disable for production)
#define OUTPUT_SERIAL_MSG           1
// Log and output delay for each sensor poll and new message
#define LOG_TIMING                  1

#if OUTPUT_SERIAL_MSG
    #define DEBUG_SERIAL(x) Serial.println(x)
#else
    #define DEBUG_SERIAL(x)
#endif

#define PUBLISH_INTERVAL_MS         5000
#define GPS_UPDATE_INTERVAL_MS      1000
#define THERMO_UPDATE_INTERVAL_MS   500

JsonMaker jsonMaker;
SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermoA(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorCan can(&SPI1, D5, D6);

Sensor *sensors[3] = {&gps, &thermoA, &can};

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
    DEBUG_SERIAL("\nNot in Message: ");

    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermoA.getTemp()) + "C");

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
        publishMessage();
    }
}



