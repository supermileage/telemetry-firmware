#include "Particle.h"
#include "Arduino.h"
#include "JsonMaker.h"
#include "Sensor_RSSI.h"
#include "Sensor_ECU.h"

#define BLINK_INTERVAL_OFF 1800
#define BLINK_INTERVAL_ON 200
#define PUBLISH_INTERVAL 5000

JsonMaker json_maker;
Sensor_RSSI rssi;
Sensor_ECU ecu(&Serial1);

uint32_t last_blink = 0; // Time of last blink
uint32_t last_publish = 0; // Time of last publish
boolean led_state = LOW;

// Handler for any new message on ID "Proto"
void proto_response(const char *event, const char *data) {
    Serial.println("Received Message on ID: " + String(event) + " - Message Data: " + String(data));
}

void setup() {

    Serial.begin(9600);
    pinMode(D7, OUTPUT);

    // Subscribe to any new messages on ID "Proto"
    Particle.subscribe("hook-response/Proto", proto_response, MY_DEVICES);

    // Setup function only runs after Boron connected in (default) Automatic mode
    Serial.println("Particle Connected!");

}

void loop() {

    digitalWrite(D7, led_state);
    
    // Blink the LED
    if (led_state & (millis() - last_blink >= BLINK_INTERVAL_ON)){
        led_state = LOW;
        last_blink = millis();
    }else if(!led_state & (millis() - last_blink >= BLINK_INTERVAL_OFF)){
        led_state = HIGH;
        last_blink = millis();
    }

    // Publish a message to Proto
    if (millis() - last_publish >= PUBLISH_INTERVAL){
        last_publish = millis();
        // Call makeJSON function
        json_maker.init();
        int start = millis();
        json_maker.add("RSSI", rssi.get());
        //Particle.publish("Proto", to_publish, PRIVATE);
        Serial.println("Time elapsed for rssi.get(): " + String(millis() - start) + "ms");
        start = millis();
        json_maker.add("RPM", ecu.getRPM());
        Serial.println("Time elapsed for ecu.getRPM(): " + String(millis() - start) + "ms");
        Serial.println("JSON Test:\n" + json_maker.get());
    }

}
