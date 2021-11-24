#include "settings.h"
#include "vehicle.h"
#include "Led.h"
#include "Button.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

void buttonPushed();

Led ledOrange(A0, 63);
Led ledBlue(D7, 255);
Led ledGreen(D8, 40);
Button button(A2, true, false, buttonPushed, NULL);

DataQueue dataQ(VEHICLE_NAME);
Dispatcher *dispatcher;

unsigned long lastPublish = 0;
bool loggingEnabled = LOGGING_ENABLED_AT_BOOT;
bool error = false;

void publishMessage() {

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

// Toggle logging enabled on and off
void buttonPushed(){
    if(loggingEnabled){
        loggingEnabled = false;
        DEBUG_SERIAL_LN("#### Logging has been DISABLED (button)");
    }else{
        loggingEnabled = true;
        DEBUG_SERIAL_LN("#### Logging has been ENABLED (button)");
    }
}

// Handle User Interface Functionality
void handleUI(){
    ledOrange.handle();
    ledBlue.handle();
    ledGreen.handle();
    button.handle();

    // Green Light Behaviour
    if(Time.isValid()){
        ledGreen.on();
    }else{
        ledGreen.flashRepeat(LED_FLASH_INTERVAL);
    }

    // Blue LED Behaviour
    if(loggingEnabled){
        if(Time.isValid() && !error){
            ledBlue.on();
        }else{
            ledBlue.flashRepeat(LED_FLASH_INTERVAL);
        }
    }else{
        ledBlue.off();
    }

    // Orange LED Behaviour
    if(error){
        ledOrange.flashRepeat(LED_FLASH_INTERVAL);
    }else{
        ledOrange.off();
    }

}

// Enable Logging Remotely
int remoteEnableLogging(String command){
    loggingEnabled = true;
    DEBUG_SERIAL_LN("#### Logging has been ENABLED (remote)");
    return 1;
}

// Disable Logging Remotely
int remoteDisableLogging(String command){
    loggingEnabled = false;
    DEBUG_SERIAL_LN("#### Logging has been DISABLED (remote)");
    return 1;
}

/**
 * SETUP
 * */
void setup() {
    if(DEBUG_SERIAL_ENABLE){
        Serial.begin(115200);
    }

    // Start i2c with clock speed of 400 KHz
    // This requires the pull-up resistors to be removed on i2c bus
    Wire.setClock(400000);
    Wire.begin();

    // Define Remote Functions
    Particle.function("enableLogging", remoteEnableLogging);
    Particle.function("disableLogging", remoteDisableLogging);

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

    if(loggingEnabled && Time.isValid()){
        dispatcher->run();
    }

    handleUI();

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        // If no valid time pulled from cellular, attempt to get valid time from GPS (should be faster)
        if(!Time.isValid()){
            if(gps.getTimeValid()){
                Time.setTime(gps.getUnixTime());
            }
        }

        lastPublish = millis();
        publishMessage();
    }
}



