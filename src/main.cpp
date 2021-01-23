#include "Particle.h"
#include "Arduino.h"
#include "DataQueue.h"

#define BLINK_INTERVAL_OFF 1800
#define BLINK_INTERVAL_ON 200
#define PUBLISH_INTERVAL 10000

SYSTEM_THREAD(ENABLED);

DataQueue* queue;

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

    queue = new DataQueue();

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

        queue->add("PROTO-RPM", random(0, 10));
        queue->add("PROTO-SPARK", random(0, 10));
        long time0 = millis();
        queue->publish("Proto", PRIVATE, WITH_ACK);
        Serial.println(millis() - time0);
    }
}
