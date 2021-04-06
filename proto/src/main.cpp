#include "Particle.h"
#include "Arduino.h"
#include "JsonMaker.h"
#include "Sensor_ECU.h"
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"

#define PUBLISH_INTERVAL_SECONDS 10

SYSTEM_THREAD(ENABLED);

JsonMaker json_maker;
Sensor_ECU ecu(&Serial1);

uint32_t last_publish = 0;


void setup() {
    Serial.begin(115200);
    pinMode(D7, OUTPUT);

    ecu.begin();

    Serial.println("Particle Connected!");
}

void loop() {

    // Check for full data frame from ECU in UART bugger
    ecu.handle();
    
    // Publish a message on the interval
    if (millis() - last_publish >= PUBLISH_INTERVAL_SECONDS*1000){
        last_publish = millis();
        // Call makeJSON function
        json_maker.refresh();
        json_maker.add("PROTO-ECT", ecu.get("ect"));
        json_maker.add("PROTO-IAT", ecu.get("iam"));
        json_maker.add("PROTO-RPM", ecu.get("rpm"));
        json_maker.add("PROTO-UBADC", ecu.get("ubadc"));
        json_maker.add("PROTO-O2S", ecu.get("o2s"));
        json_maker.add("PROTO-SPARK", ecu.get("spark"));
        // Particle.publish("Proto", json_maker.get(), PRIVATE, WITH_ACK);
        Serial.println("New JSON Message: " + json_maker.get());
    }



}



