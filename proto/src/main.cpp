#include "Particle.h"
#include "Arduino.h"
#include "JsonMaker.h"
#include "Sensor_ECU.h"

#define PUBLISH_INTERVAL 3000

SYSTEM_THREAD(ENABLED);

void onSerialData();
JsonMaker json_maker;
Sensor_ECU ecu(&Serial1);


uint32_t last_publish = 0; // Time of last publish


void setup() {
    Serial.begin(115200);
    pinMode(D7, OUTPUT);

    ecu.begin();

    // Setup function only runs after Boron connected in (default) Automatic mode
    Serial.println("Particle Connected!");
}

void loop() {

    ecu.handle();
    
    //Publish a message to Proto
    if (millis() - last_publish >= PUBLISH_INTERVAL){
        last_publish = millis();
        //Call makeJSON function
        json_maker.refresh();
        json_maker.add("PROTO-RPM", ecu.getRPM());
        json_maker.add("PROTO-SPARK", ecu.getSpark());
        //Particle.publish("Proto", json_maker.get(), PRIVATE, WITH_ACK);
        Serial.print("New JSON Message: " + json_maker.get() + "\n");
    }



}



