#include "fc_globals.h"

// sensor definitions
SensorGps gps(GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);

// command definitions
LogCommand<SensorGps, float> gpsLat(&dataQ, &gps, "lat", &SensorGps::getLatitude, 1);
LogCommand<SensorGps, float> gpsLong(&dataQ, &gps, "long", &SensorGps::getLongitude, 1);
LogCommand<SensorGps, float> gpsVertAccel(&dataQ, &gps, "v-accel", &SensorGps::getVerticalAcceleration, 2);
LogCommand<SensorGps, float> gpsHorAccel(&dataQ, &gps, "h-accel", &SensorGps::getHorizontalAcceleration, 2);
LogCommand<SensorThermo, double> thermoTemp1(&dataQ, &thermo1, "temp1", &SensorThermo::getTemp, 5);
LogCommand<SensorThermo, double> thermoTemp2(&dataQ, &thermo2, "temp2", &SensorThermo::getTemp, 5);

// array definitions: update counts if you make changes here
Sensor *sensors[] = { &gps, &thermo1, &thermo2 };
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &gpsVertAccel, &gpsHorAccel, &thermoTemp1, &thermoTemp2 };
uint16_t sensor_count = 3;
uint16_t command_count = 6;


// SerialDebugPublishing namespace definitions

/**
 * Publishes a new message to Particle Cloud
**/
void SerialDebugPublishing::publishMessage() {
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

void SerialDebugPublishing::setupMessage() {
    DEBUG_SERIAL("TELEMETRY ONLINE - FC");
}