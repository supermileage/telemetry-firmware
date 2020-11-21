#include "DataGenerator.h"
#include "JsonMaker.h"

#define BLINK_INTERVAL_OFF 1800
#define BLINK_INTERVAL_ON 200
#define PUBLISH_INTERVAL 30000

JsonMaker json_maker;
DataGenerator data_generator;

uint32_t last_blink = 0; // Time of last blink
uint32_t last_publish = 0; // Time of last publish
boolean led_state = LOW;

//SYSTEM_MODE(MANUAL);

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
        json_maker.clear();
        json_maker.add("PROTO-RPM", data_generator.get());
        String to_publish = json_maker.get();
        Particle.publish("Proto", to_publish, PRIVATE);
        Serial.println("Sent message to ID: Proto - Message Data: " + to_publish);
    }

}
