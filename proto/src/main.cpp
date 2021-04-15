#include "Particle.h"
#include "JsonMaker.h"
#include "SensorEcu.h"
#include "SensorGps.h"
#include "SensorThermo.h"

#define PUBLISH_INTERVAL_SECONDS 5

SYSTEM_THREAD(ENABLED);

JsonMaker jsonMaker;
SensorEcu ecu(&Serial1);
SensorGps gps(1000);
SensorThermo thermoA(&SPI, A5, 1000);

uint32_t lastPublish = 0;


void setup() {
    Serial.begin(115200);

    ecu.begin();
    gps.begin();

    Serial.println("Particle Connected!");
}

void loop() {

    // Check for full data frame from ECU in UART buffer
    ecu.handle();
    gps.handle();
    thermoA.handle();

    // Publish a message on the interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_SECONDS*1000){
        lastPublish = millis();
        // Call makeJSON function
        jsonMaker.refresh();
        jsonMaker.add("PROTO-ECT", ecu.getECT());
        jsonMaker.add("PROTO-IAT", ecu.getIAT());
        jsonMaker.add("PROTO-RPM", ecu.getRPM());
        jsonMaker.add("PROTO-UBADC", ecu.getUbAdc());
        jsonMaker.add("PROTO-O2S", ecu.getO2S());
        jsonMaker.add("PROTO-SPARK", ecu.getSpark());
        // Particle.publish("Proto", jsonMaker.get(), PRIVATE, WITH_ACK);
        Serial.println("New JSON Message: " + jsonMaker.get());
        Serial.println(gps.getSentence());
        Serial.print("Current Temperature (Thermo1): "); Serial.print(thermoA.getTemp()); Serial.println("C");
    }



}



