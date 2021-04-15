#include "Particle.h"
#include "JsonMaker.h"
#include "SensorEcu.h"
#include "SensorGps.h"
#include "SensorThermo.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Disable publish when not necessary during development to reduce data usage
#define PUBLISH_ENABLED             1
#define PUBLISH_INTERVAL_MS         5000

#define GPS_UPDATE_INTERVAL_MS      1000
#define THERMO_UPDATE_INTERVAL_MS   500

JsonMaker jsonMaker;
SensorEcu ecu(&Serial1);
SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermoA(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);

uint32_t lastPublish = 0;

/**
 * Publishes a new message to Particle Cloud
 * */
void publishMessage(){
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

    if(PUBLISH_ENABLED){
        // Publish to Particle Cloud
        Particle.publish("Proto", jsonMaker.get(), PRIVATE, WITH_ACK);
        Serial.println("Publish - ENABLED - Message: ");
    }else{
        Serial.println("Publish - DISABLED - Message: ");
    }

    Serial.println("New JSON Message: " + jsonMaker.get());

    // Any sensors that are working but not yet packaged for publish
    Serial.println("Not in Message: ");
    Serial.print("Current Temperature (Thermo1): "); Serial.print(thermoA.getTemp()); Serial.println("C");
    Serial.println();
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

}

/**
 * 
 * LOOP
 * 
 * */
void loop() {
    ecu.handle();
    gps.handle();
    thermoA.handle();

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        lastPublish = millis();
        publishMessage();
    }
}



