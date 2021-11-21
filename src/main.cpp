#include "settings.h"
#include "vehicle.h"
#include "Led.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

Led ledOrange(A0, 63);
Led ledBlue(D7, 255);
Led ledGreen(D8, 40);

DataQueue dataQ(VEHICLE_NAME);
Dispatcher *dispatcher;
unsigned long lastPublish = 0;
boolean loggingEnabled = LOGGING_ENABLED_AT_BOOT;

void publishMessage() {

    // dataQ.add("property", "value");

    DEBUG_SERIAL_LN("------------------------");
    DEBUG_SERIAL_LN("Time: " + Time.timeStr());
    if(loggingEnabled){
        DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Logging ENABLED - Message: ");
        // Publish to Particle Cloud
        DEBUG_SERIAL_LN(dataQ.publish("BQIngestion", PRIVATE, WITH_ACK));
    }else{
        DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Logging DISABLED");
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

    // Green LED flashes on boot-up
    ledGreen.flashRepeat(500);
    // Blue LED flashes on boot-up if logging enabled
    if(loggingEnabled){
        ledBlue.flashRepeat(500);
    }

    if(DEBUG_SERIAL_ENABLE){
        Serial.begin(115200);
    }

    // Start i2c with clock speed of 400 KHz
    // This requires the pull-up resistors to be removed on i2c bus
    Wire.setClock(400000);
    Wire.begin();

    Time.zone(TIME_ZONE);

    for (unsigned i = 0; i < sensor_count; i++) {
        sensors[i]->begin();
    }

    DispatcherBuilder builder(commands, command_count, &dataQ);
    dispatcher = builder.build();

    DEBUG_SERIAL_LN("TELEMETRY ONLINE - " + String(VEHICLE_NAME));
}

/**
 * LOOP
 * */
void loop() {
    // Sensor Handlers
    for (unsigned i = 0; i < sensor_count; i++) {
        sensors[i]->handle();
    }

    dataQ.loop();
    if(loggingEnabled){
        dispatcher->run();
    }
    
    if(digitalRead(A2)){
        loggingEnabled = !loggingEnabled;
    }

    // LED Handlers
    ledOrange.handle();
    ledBlue.handle();
    ledGreen.handle();

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        // If no valid time pulled from cellular, attempt to get valid time from GPS (should be faster)
        if(!Time.isValid()){
            if(gps.getTimeValid()){
                Time.setTime(gps.getUnixTime());
            }
        }else{
            ledGreen.on();
            ledBlue.on();
        }

        lastPublish = millis();
        publishMessage();
    }
}



