#include "Particle.h"
#include "Arduino.h"
#include "JsonMaker.h"

#define BLINK_INTERVAL_OFF 1800
#define BLINK_INTERVAL_ON 200
#define PUBLISH_INTERVAL 10000

SYSTEM_THREAD(ENABLED);

JsonMaker json_maker;

uint32_t last_publish = 0; // Time of last publish



void setup() {

    // Setup function only runs after Boron connected in (default) Automatic mode
    Serial.println("Particle Connected!");

}

void loop() {

    //Publish a message
    if (millis() - last_publish >= PUBLISH_INTERVAL){
        last_publish = millis();
    }

}


