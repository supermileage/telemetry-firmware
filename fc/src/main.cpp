#include "globals.h"
#include "Particle.h"
#include "DispatcherFactory.h"
#include "Led.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

Led led_orange(A0, 63);
// Blue LED to flash on startup, go solid when valid time has been established
Led led_blue(D7, 255);
Led led_green(D8, 40);

Dispatcher *dispatcher;

unsigned long lastPublish = 0;

/**
 * Publishes a new message to Particle Cloud
 * */
void publishMessage() {
    long start, json_build_time;
    if (DEBUG_CPU_TIME) {
        start = micros();
    }

    if (DEBUG_CPU_TIME) {
        json_build_time = micros() - start;
    }

    DEBUG_SERIAL("------------------------");
    if(PUBLISH_ENABLED){
        DEBUG_SERIAL("Publish - ENABLED - Message: ");
        // Publish to Particle Cloud
        DEBUG_SERIAL(dataQ.publish("Proto", PRIVATE, WITH_ACK));
    }else{
        DEBUG_SERIAL("Publish - DISABLED - Message: ");
        DEBUG_SERIAL(dataQ.resetData());
    }

    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("\nNot in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermo1.getTemp()) + "C");
    DEBUG_SERIAL("Current Temperature (Thermo2): " + String(thermo2.getTemp()) + "C");
    DEBUG_SERIAL("Current Time (UTC): " + Time.timeStr());
    DEBUG_SERIAL();
    
    if(DEBUG_MEM){
        DEBUG_SERIAL("\nFREE RAM: " + String(System.freeMemory()) + "B / 128000B");
    }

    // Output CPU time in microseconds spent on each task
    if (DEBUG_CPU_TIME) {
        DEBUG_SERIAL("\nCPU Time:");
        DEBUG_SERIAL("Build JSON Message: " + String(json_build_time) + "us");
        for (Sensor *s : sensors) {
            DEBUG_SERIAL(s->getHumanName() + " polling: " + String(s->getLongestHandleTime()) + "us");
        }
        DEBUG_SERIAL();
    }
}

/**
 * 
 * SETUP
 * 
 * */
void setup() {
    if(DEBUG_SERIAL_ENABLE){
        Serial.begin(115200);
    }

    // Start i2c with clock speed of 400 KHz
    // This requires the pull-up resistors to be removed on i2c bus
    Wire.setClock(400000);
    Wire.begin();

    Time.zone(TIME_ZONE);

    for (Sensor *s : sensors) {
        s->begin();
    }

    // commands are define in globals.cpp
    DispatcherFactory factory(commands, &dataQ);
    dispatcher = factory.build();

    led_blue.flashRepeat(500);

    DEBUG_SERIAL("TELEMETRY ONLINE - FC");
}

/**
 * 
 * LOOP
 * 
 * */
void loop() {
    // Sensor Handlers
    for (Sensor *s : sensors) {
        if (DEBUG_CPU_TIME) {
            s->benchmarkedHandle();
        } else {
            s->handle();
        }
    }

    dataQ.loop();
    dispatcher->run();

    // LED Handlers
    led_orange.handle();
    led_blue.handle();
    led_green.handle();

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        // If no valid time pulled from cellular, attempt to get valid time from GPS (should be faster)
        if(!Time.isValid()){
            if(gps.getTimeValid()){
                led_blue.on();
                Time.setTime(gps.getUnixTime());
            }
        }else{
            led_blue.on();
        }

        lastPublish = millis();
        publishMessage();
    }

    
}