#include "settings.h"
#include "vehicle.h"
#include "Led.h"
#include "TimeLib.h"
#include "Button.h"
#include "Handler.h"
#include "Handleable.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Forward declarations for callback functions
void buttonPushed();
void buttonHeld();
void publish(String payload, DataQueue::PublishData status);
void timeValidCallback();

// Construct all Handleables
Led ledOrange(A0, 63);
Led ledBlue(D7, 255);
Led ledGreen(D8, 40);
Button button(A2, true, false, buttonPushed, NULL, buttonHeld);
DataQueue dataQ(VEHICLE_NAME, publish);
TimeLib timeLib(timeValidCallback);
LoggingDispatcher *dispatcher;

bool loggingEnabled = LOGGING_EN_AT_BOOT;
bool loggingError = false;
bool gpsOverride = false;
long unsigned int lastDebugSensor = 0;
unsigned long lastPublish = 0;

#pragma region DebugMessages

// Publish a message
void publish(String payload, DataQueue::PublishData data) {
    loggingError = false;

    // publish status messages
    if (data.status == DataQueue::DataBufferOverflow) {
        DEBUG_SERIAL_LN("ERROR: Json String has Exceeded Maximum Size of " + String(JSON_BUFFER_SIZE) + " Bytes");
        loggingError = true;
    } else if (data.status == DataQueue::JsonDocumentOverflow) {
        DEBUG_SERIAL_LN("ERROR: JsonDocument has overflowed due to complexity of unserialized Json in DataQueue::_jsonDocument");
        DEBUG_SERIAL_LN("Increase JSON_DOCUMENT_SIZE to account for this complexity");
        DEBUG_SERIAL_LN(" - memory currently allocated for JsonDocument: " + String(JSON_DOCUMENT_SIZE));
        DEBUG_SERIAL_LN(" - memory usage of JsonDocument: " + String(data.jsonDocumentSize) + " bytes for Json string of " + String(payload.length()) + " bytes");
        loggingError = true;
    }  else if (data.status == DataQueue::PublishingAtMaxFrequency) {
        DEBUG_SERIAL_LN("WARNING: Currently Publishing at Max Frequency");
    }
    if (dataQ.isCacheFull()) {
        DEBUG_SERIAL_LN("");
        DEBUG_SERIAL_LN("WARNING: Publish Queue is full");
    }

    DEBUG_SERIAL_LN("");
    DEBUG_SERIAL_LN("---- PUBLISH MESSAGE ----");
    DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - Publish " + (PUBLISH_EN ? "ENABLED" : "DISABLED") + " - " + timeLib.getTimeString());
    DEBUG_SERIAL_LN(payload);
    DEBUG_SERIAL_LN("");
    DEBUG_SERIAL("Publish Queue Size: " + String(dataQ.getNumEventsInQueue()) + "/100");
    DEBUG_SERIAL(" -- JsonString: " + String(payload.length()) + "/" + String(JSON_BUFFER_SIZE) + " bytes");
    DEBUG_SERIAL_LN(" -- JsonDocument: " + String(data.jsonDocumentSize) + "/" + String(JSON_DOCUMENT_SIZE)  + " bytes");
    DEBUG_SERIAL_LN("");
}

// Output sensor data over serial
void debugSensors(){

    DEBUG_SERIAL_LN("---- SENSOR DATA ----");
    DEBUG_SERIAL_LN(String(VEHICLE_NAME) + " - " + timeLib.getTimeString());
    CurrentVehicle::debugSensorData();
    if(gpsOverride) {
        DEBUG_SERIAL_LN("!!WARNING!! GPS GREENLIST OVERRIDE IS ENABLED");
    }
    DEBUG_SERIAL_LN("Free Memory: " + String(System.freeMemory()/1000) + "kB / 128kB");
    DEBUG_SERIAL_LN("");
    
}

#pragma endregion

#pragma region Logging

// Action to take when time becomes valid
void timeValidCallback() {

    DEBUG_SERIAL("#### TIME VALID ");
    if(loggingEnabled){
        dispatcher->setLoggingEnabled(TRUE);
    }

}

// Enable logging of sensor data
void enableLogging() {

    if(!loggingEnabled) {
        loggingEnabled = TRUE;
        dispatcher->setLoggingEnabled(TRUE);
        DEBUG_SERIAL_LN("Logging has been ENABLED\n");
    }

}

// Disable logging of sensor data
void disableLogging() {
    if(loggingEnabled) {
        loggingEnabled = FALSE;
        dispatcher->setLoggingEnabled(FALSE);
        DEBUG_SERIAL_LN("Logging has been DISABLED\n");
    }
}

#pragma endregion

#pragma region UI

// Action to take when button pushed
void buttonPushed(){
    DEBUG_SERIAL("#### BUTTON PUSHED - ");
    if(loggingEnabled){
        disableLogging();
    }else{
        enableLogging();
    }
}

// Action to take when button held for 2s
void buttonHeld() {
    CurrentVehicle::toggleGpsOverride();
    gpsOverride = !gpsOverride;
    DEBUG_SERIAL("#### BUTTON HELD - ");
    if(gpsOverride) {
        DEBUG_SERIAL_LN("GPS GREENLIST OVERRIDE ENABLED - !!WARNING!! YOUR GPS POSITION COULD BE RECORDED OUTSIDE OF A DESIGNATED ZONE");
    } else {
        DEBUG_SERIAL_LN("GPS GREENLIST OVERRIDE DISABLED");
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
        if(Time.isValid() && !loggingError){
            ledBlue.on();
        }else{
            ledBlue.flashRepeat(LED_FLASH_INT);
        }
    } else {
        ledBlue.off();
    }

    // Orange LED Behaviour
    if(loggingError || gpsOverride){
        ledOrange.flashRepeat(LED_FLASH_INT);
    }else{
        ledOrange.off();
    }

}

#pragma endregion

#pragma region RemoteFunctions

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

// Restart BMS Remotely
int remoteRestartTinyBms(String command) {

    DEBUG_SERIAL_LN("#### REMOTE - Sent TinyBMS Restart Command\n");
    CurrentVehicle::restartTinyBms();
    return 1;

}

#pragma endregion

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
    Particle.function("restartTinyBms", remoteRestartTinyBms);

    Time.zone(TIME_ZONE);

    dispatcher = CurrentVehicle::buildLoggingDispatcher();

    // Begin all handleables
    Handler::instance().begin();

	Particle.connect();
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

