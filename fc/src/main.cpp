#include "Particle.h"
#include "settings.h"
#include "DataQueue.h"
#include "Led.h"

#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);

Sensor *sensors[3] = {&gps, &thermo1, &thermo2};

Led led_orange(A0, 63);
// Blue LED to flash on startup, go solid when valid time has been established
Led led_blue(D7, 255);
Led led_green(D8, 40);

DataQueue dataQ;

uint32_t lastPublish = 0;

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
        DEBUG_SERIAL(dataQ.publish("FC", PRIVATE, WITH_ACK));
    }else{
        DEBUG_SERIAL("Publish - DISABLED - Message: ");
        DEBUG_SERIAL(dataQ.resetData());
    }

    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("\nNot in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermo1.getTemp()) + "C");
    DEBUG_SERIAL("Current Temperature (Thermo2): " + String(thermo2.getTemp()) + "C");
    DEBUG_SERIAL("GPS Sentence: " + gps.getSentence());
    DEBUG_SERIAL("Current Speed: " + String(gps.getSpeedKph()) + "KM/h");    
    DEBUG_SERIAL("Current Time (UTC): " + Time.timeStr());
    DEBUG_SERIAL();
    
    if(DEBUG_MEM){
        DEBUG_SERIAL("\nFREE RAM: " + String(System.freeMemory()) + "B / 80000B");
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

    Time.zone(TIME_ZONE);

    for (Sensor *s : sensors) {
        s->begin();
    }

    led_blue.flashRepeat(500);

    DEBUG_SERIAL("TELEMETRY ONLINE - FUEL CELL");
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
                Time.setTime(gps.getTime());
            }
        }else{
            led_blue.on();
        }

        lastPublish = millis();
        publishMessage();
    }

    
}


