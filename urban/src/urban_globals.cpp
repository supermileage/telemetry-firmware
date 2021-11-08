#include "urban_globals.h"

// Object Definitions for Urban Sensors and Sensor Data Telemetry

// sensor definitions
SensorGps gps(GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);

// command definitions
LogCommand<SensorGps, float> gpsLat(&dataQ, &gps, "URBAN-Latitude", &SensorGps::getLatitude, 1);
LogCommand<SensorGps, float> gpsLong(&dataQ, &gps, "URBAN-Longitude", &SensorGps::getLongitude, 1);
LogCommand<SensorThermo, double> thermoTemp1(&dataQ, &thermo1, "URBAN-Temperature", &SensorThermo::getTemp, 5);

// array definitions: update counts if you make changes here
Sensor *sensors[] = { &gps, &thermo1, &thermo2 };
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &thermoTemp1 };
uint16_t sensor_count = 3;
uint16_t command_count = 3;


// SerialDebugPublishing namespace definitions

/**
 * Publishes a new message to Particle Cloud for urban
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

    // Data NOT packaged for publish
    DEBUG_SERIAL("\nNot in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo2): " + String(thermo2.getTemp()) + "C");
    DEBUG_SERIAL("Current Speed: " + String(gps.getHorizontalSpeed()) + "KM/h");    
    DEBUG_SERIAL("Current Time (UTC): " + Time.timeStr());

    // for(int i = 0; i < can.getNumIds(); i++){
    //     String output = "CAN ID: 0x" + String(can.getId(i), HEX) + " - CAN Data:";
    //     uint8_t canDataLength = can.getDataLen(i);
    //     unsigned char* canData = can.getData(i);
    //     for(int k = 0; k < canDataLength; k++){
    //         output += " 0x";
    //         output += String(canData[k], HEX);
    //     }
    //     DEBUG_SERIAL(output);
    // }

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
 * Prints urbans's setup message
**/
void SerialDebugPublishing::setupMessage() {
        DEBUG_SERIAL("TELEMETRY ONLINE - URBAN");
}