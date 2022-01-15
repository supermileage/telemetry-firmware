#include "settings.h"
#include "vehicle.h"
#include "Led.h"
#include "TimeLib.h"
#include "Button.h"
#include "Handler.h"
#include "Handleable.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Forward declarations for callback functions
void buttonPushed();
void publish(String payload, DataQueue::PublishStatus status);
void timeValidCallback();

// Construct all Handleables
Led ledOrange(A0, 63);
Led ledBlue(D7, 255);
Led ledGreen(D8, 40);
Button button(A2, true, false, buttonPushed, NULL);
DataQueue dataQ(VEHICLE_NAME, publish);
TimeLib timeLib(timeValidCallback);
Dispatcher *dispatcher;

bool loggingEnabled = LOGGING_EN_AT_BOOT;
bool error = false;
long unsigned int lastDebugSensor = 0;
unsigned long lastPublish = 0;

// Publish a message
void publish(String payload, DataQueue::PublishStatus status) {
    if(status == DataQueue::DataBufferOverflow || dataQ.isCacheFull()) {
        error = true;
        if(status == DataQueue::DataBufferOverflow) {
            DEBUG_SERIAL_LN("ERROR: Json Writer Data Buffer has Overflowed!");
        }
        if(dataQ.isCacheFull()) {
            DEBUG_SERIAL_LN("ERROR: Data Queue is full!");
        }
    } else {
        error = false;
    }
    
    if(status == DataQueue::PublishingAtMaxFrequency) {
        DEBUG_SERIAL_LN("WARNING: Currently Publishing at Max Frequency");
    }

    DEBUG_SERIAL_LN("---- PUBLISH MESSAGE ----");
    DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Publish " + (PUBLISH_EN ? "ENABLED" : "DISABLED") + " - " + timeLib.getTimeString());
    DEBUG_SERIAL_LN(payload);
    DEBUG_SERIAL_LN("");
    DEBUG_SERIAL_LN("Publish Queue Size: " + String(dataQ.getNumEventsInQueue()));
    DEBUG_SERIAL_LN("");

}

// Output sensor data over serial
void debugSensors(){

    DEBUG_SERIAL_LN("---- SENSOR DATA ----");
    DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - " + timeLib.getTimeString());
    CurrentVehicle::debugSensorData();

    DEBUG_SERIAL_LN("Free Memory: " + String(System.freeMemory()/1000) + "kB / 128kB");
    DEBUG_SERIAL_LN("");
    
}

// Action to take when time becomes valid
void timeValidCallback() {

    DEBUG_SERIAL("#### TIME VALID ");
    if(loggingEnabled){
        dispatcher->setEnableLogging(TRUE);
    }

}

// Enable logging of sensor data
void enableLogging() {

    if(!loggingEnabled) {
        loggingEnabled = TRUE;
        dispatcher->setEnableLogging(TRUE);
        DEBUG_SERIAL_LN("Logging has been ENABLED\n");
    }

}

// Disable logging of sensor data
void disableLogging() {

    if(loggingEnabled) {
        loggingEnabled = FALSE;
        dispatcher->setEnableLogging(FALSE);
        DEBUG_SERIAL_LN("Logging has been DISABLED\n");
    }

}

// Action to take when button pushed
void buttonPushed(){
    DEBUG_SERIAL("#### BUTTON PUSHED - ");
    if(loggingEnabled){
        disableLogging();
    }else{
        enableLogging();
    }
}

// Handle User Interface Functionality
void handleUI(){

    // Green Light Behaviour
    if(Time.isValid()){
        ledGreen.on();
    }else{
        ledGreen.flashRepeat(LED_FLASH_INT);
    }

    // Blue LED Behaviour
    if(loggingEnabled){
        if(Time.isValid() && !error){
            ledBlue.on();
        }else{
            ledBlue.flashRepeat(LED_FLASH_INT);
        }
    } else {
        ledBlue.off();
    }

    // Orange LED Behaviour
    if(error){
        ledOrange.flashRepeat(LED_FLASH_INT);
    }else{
        ledOrange.off();
    }

}

// Reboot Remotely
int remoteReset(String command) {

    DEBUG_SERIAL_LN("#### REMOTE - Boron has been RESET\n");
    System.reset();
    return 1;

}

// Enable Logging Remotely
int remoteEnableLogging(String command){

    DEBUG_SERIAL("#### REMOTE - ");
    enableLogging();
    return 1;

}

// Disable Logging Remotely
int remoteDisableLogging(String command){

    DEBUG_SERIAL("#### REMOTE - ");
    disableLogging();
    return 1;

}

/**
 * SETUP
 * */
void setup() {

    if(DEBUG_SERIAL_EN){
        Serial.begin(115200);
    }

    // Start i2c with clock speed of 400 KHz
    // This requires the pull-up resistors to be removed on i2c bus
    Wire.setClock(400000);
    Wire.begin();

    // Define Remote Functions
    Particle.function("remoteReset", remoteReset);
    Particle.function("enableLogging", remoteEnableLogging);
    Particle.function("disableLogging", remoteDisableLogging);

    Time.zone(TIME_ZONE);

    dispatcher = CurrentVehicle::buildDispatcher();

    // Begin all handleables
    Handler::instance().begin();

    DEBUG_SERIAL_LN("---- TELEMETRY ONLINE - " + String(VEHICLE_NAME) + " ----\n");

}

/**
 * LOOP
 * */
void loop() {

    // Run all handleables
    Handler::instance().handle();

    handleUI();

    if(DEBUG_SENSOR_INT && millis() > lastDebugSensor + (DEBUG_SENSOR_INT * 1000)){
        lastDebugSensor = millis();
        debugSensors();
    }

}

