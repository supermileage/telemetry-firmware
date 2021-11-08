#include "proto_globals.h"

// sensor definitions
SensorGps gps(GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);
SensorEcu ecu(&Serial1);

// command definitions
LogCommand<SensorEcu, float> ecuEct(&dataQ, &ecu, "PROTO-ECT", &SensorEcu::getECT, 5);
LogCommand<SensorEcu, float> ecuIat(&dataQ, &ecu, "PROTO-IAT", &SensorEcu::getIAT, 5);
LogCommand<SensorEcu, float> ecuRpm(&dataQ, &ecu, "PROTO-RPM", &SensorEcu::getRPM, 5);
LogCommand<SensorEcu, float> ecuUbAdc(&dataQ, &ecu, "PROTO-UBADC", &SensorEcu::getUbAdc, 5);
LogCommand<SensorEcu, float> ecu02S(&dataQ, &ecu, "PROTO-O2S", &SensorEcu::getO2S, 5);
LogCommand<SensorEcu, float> ecuSpark(&dataQ, &ecu, "PROTO-SPARK", &SensorEcu::getSpark, 5);
LogCommand<SensorGps, float> gpsLat(&dataQ, &gps, "PROTO-Latitude", &SensorGps::getLatitude, 1);
LogCommand<SensorGps, float> gpsHvel(&dataQ, &gps, "PROTO-Speed", &SensorGps::getHorizontalSpeed, 1);

// array definitions: update counts if you make changes here
Sensor *sensors[] = {&ecu, &gps, &thermo1, &thermo2};
IntervalCommand *commands[] = { &ecuEct, &ecuIat, &ecuRpm, &ecuUbAdc, &ecu02S, &ecuSpark, &gpsLat, &gpsHvel };
uint16_t sensor_count = 4;
uint16_t command_count = 8;


// SerialDebugPublishing namespace definitions

/**
 * Publishes a new message to Particle Cloud for proto
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

/**
 * Prints proto's setup message
**/
void SerialDebugPublishing::setupMessage() {
    DEBUG_SERIAL("TELEMETRY ONLINE - PROTO");
}