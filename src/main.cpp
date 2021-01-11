#include "Particle.h"
#include "Arduino.h"
#include "JsonMaker.h"
#include "Sensor_RSSI.h"
#include "Sensor_ECU.h"

#define BLINK_INTERVAL_OFF 1800
#define BLINK_INTERVAL_ON 200
#define PUBLISH_INTERVAL 10000

SYSTEM_THREAD(ENABLED);

void onSerialData();
JsonMaker json_maker;
Sensor_RSSI rssi;
Sensor_ECU ecu(&Serial1);


uint32_t last_blink = 0; // Time of last blink
uint32_t last_publish = 0; // Time of last publish
boolean led_state = LOW;

Timer timer(1, onSerialData);

// Handler for any new message on ID "Proto"
void proto_response(const char *event, const char *data) {
    Serial.println("Received Message on ID: " + String(event) + " - Message Data: " + String(data));
}

void setup() {
    pinMode(D7, OUTPUT);

    ecu.begin();
    timer.start();

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

    //Publish a message to Proto
    if (millis() - last_publish >= PUBLISH_INTERVAL){
        last_publish = millis();
        //Call makeJSON function
        json_maker.init();
        //json_maker.add("RSSI", rssi.get());
        json_maker.add("PROTO-RPM", ecu.getRPM());
        json_maker.add("PROTO-SPARK", ecu.getSpark());
        Particle.publish("Proto", json_maker.get(), PRIVATE, WITH_ACK);
    }

}

void onSerialData()
{
    ecu.onSerialData();
}

