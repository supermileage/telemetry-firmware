#include "settings.h"
#include "vehicle.h"
#include "Led.h"
#include "TimeLib.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

Led led_orange(A0, 63);
Led led_blue(D7, 255);
Led led_green(D8, 40);

DataQueue dataQ(VEHICLE_NAME);
Dispatcher *dispatcher;
TimeLib timeLib;
unsigned long lastPublish = 0;

void publishMessage() {

    // dataQ.add("property", "value");

    DEBUG_SERIAL_LN("------------------------");
    DEBUG_SERIAL_LN("Time: " + Time.timeStr());
    if(PUBLISH_ENABLED){
        DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Publish ENABLED - Message: ");
        // Publish to Particle Cloud
        DEBUG_SERIAL_LN(dataQ.publish("BQIngestion", PRIVATE, WITH_ACK));
    }else{
        DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Publish DISABLED - Message: ");
        DEBUG_SERIAL_LN(dataQ.resetData());
    }
    
    if(DEBUG_SENSOR_ENABLE){
        CurrentVehicle::debugSensorData();
    }

    if(DEBUG_MEM){
        DEBUG_SERIAL_LN("\nFREE RAM: " + String(System.freeMemory()) + "B / 128000B");
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

    DispatcherBuilder builder(commands, &dataQ);
    dispatcher = builder.build();

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
    if(Time.isValid()) {   
        dispatcher->run();   
    }

    // LED Handlers
    led_orange.handle();
    led_blue.handle();
    led_green.handle();

    timeLib.handle();

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
    
        lastPublish = millis();
        publishMessage();
    }
}
/*
if("SOMETHING == TRUE") {
    System.reset();
    System.reset("ANY PARAMETERS???"); //Optional System.resetReason(), doubt we will use it
}

*/