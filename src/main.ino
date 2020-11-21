#include "DataGenerator.h"

#define BLINK_INTERVAL_OFF 1800
#define BLINK_INTERVAL_ON 200
#define PUBLISH_INTERVAL 30000


DataGenerator data_generator;

uint32_t last_blink = 0; // Time of last blink
uint32_t last_publish = 0; // Time of last publish
boolean led_state = LOW;

//SYSTEM_MODE(MANUAL);

// Handler for any new message on ID "Proto"
void proto_response(const char *event, const char *data) {
    Serial.println("Received Message on ID: " + String(event) + " - Message Data: " + String(data));
}

/**
 * Creates a JSON string for updating Web Console
 * 
 * Precondition: id is not null
 * 
 * @param id : the ID of the sensor
 * @param value : the value to be updated to the ID
 * @return a JSON object represented as a JSON string.
 **/
String makeJSON(String id, int value){
    
    char buf[500]; // Allocate 500 bytes of memory for string

    // EXAMPLE
    memset(buf, 0, sizeof(buf)); //Clear array
    JSONBufferWriter writer(buf, sizeof(buf) - 1); // Create JSONBufferWriter object called writer
    writer.beginObject();
        writer.name("time").value((int)Time.now());
        writer.name("d").beginArray();
            writer.beginObject();
                writer.name("t").value(id);
                writer.name("d").value(value);
            writer.endObject();
        writer.endArray();
    writer.endObject();
    
    /* Expected Output:
    {
        "time":Time.now(),
        "d":[
            {
                "t":"PROTO-SPARK",
                "d":num
            }
        ]
    }
    */

    //Converts character array to a string
    return String(buf);
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
        String to_publish = makeJSON("PROTO-RPM", data_generator.get());
        Particle.publish("Proto", to_publish, PRIVATE);
        Serial.println("Sent message to ID: Proto - Message Data: " + to_publish);
    }

}
