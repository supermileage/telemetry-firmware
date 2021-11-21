#include "settings.h"
#include "vehicle.h"
#include "Led.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

Led led_orange(A0, 63);
Led led_blue(D7, 255);
Led led_green(D8, 40);

void publishCallback(String payload, DataQueue::PublishStatus status);

DataQueue dataQ(VEHICLE_NAME, publishCallback);
Dispatcher *dispatcher;

void publishMessage(String payload) {
    DEBUG_SERIAL_LN("------------------------");
    DEBUG_SERIAL_LN("Time: " + Time.timeStr());

    if(PUBLISH_ENABLED){
        DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Publish ENABLED - Message: ");
    }else{
        DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Publish DISABLED - Message: ");
    }

    DEBUG_SERIAL_LN(payload);
    
    if(DEBUG_SENSOR_ENABLE){
        CurrentVehicle::debugSensorData();
    }

    if(DEBUG_MEM){
        DEBUG_SERIAL_LN("\nFREE RAM: " + String(System.freeMemory()) + "B / 128000B");
    }
}

// allows us to define behavior depending on publish status
void publishCallback(String payload, DataQueue::PublishStatus status) {
    switch (status) {
        case DataQueue::PublishingAtMaxFrequency:
            DEBUG_SERIAL_LN("CURRENTLY PUBLISHING AT MAX FREQUENCY");
            publishMessage(payload);
            break;
        case DataQueue::DataBufferOverflow:
            DEBUG_SERIAL_LN("PUBLISH ERROR: JSON WRITER BUFFER HAS OVERFLOWED");
            // implement some handling behavior
            break;
        default:
            publishMessage(payload);
    }
}

/**
 * SETUP
 * */
void setup() {
    // A2 is the publish button input, setting up as input for safety
    pinMode(A2,INPUT_PULLDOWN);

    if(DEBUG_SERIAL_ENABLE){
        Serial.begin(115200);
    }

    // Start i2c with clock speed of 400 KHz
    // This requires the pull-up resistors to be removed on i2c bus
    Wire.setClock(400000);
    Wire.begin();

    Time.zone(TIME_ZONE);

    for (unsigned i = 0; sensors[i]; i++) {
        sensors[i]->begin();
    }

    dispatcher = CurrentVehicle::buildDispatcher();

    DEBUG_SERIAL_LN("TELEMETRY ONLINE - " + String(VEHICLE_NAME));
}

/**
 * LOOP
 * */
void loop() {
    // Sensor Handlers
    for (unsigned i = 0; sensors[i]; i++) {
        sensors[i]->handle();
    }

    dataQ.loop();
    dispatcher->loop();

    // LED Handlers
    led_orange.handle();
    led_blue.handle();
    led_green.handle();
}



