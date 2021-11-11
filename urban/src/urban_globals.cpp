#include "urban_globals.h"

// Object Definitions for Urban Sensors and Sensor Data Telemetry

// sensor definitions
SensorGps gps(GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);

// command definitions
SensorCommand<SensorGps, float> gpsLat(&dataQ, &gps, "URBAN-Latitude", &SensorGps::getLatitude, 1);
SensorCommand<SensorGps, float> gpsLong(&dataQ, &gps, "URBAN-Longitude", &SensorGps::getLongitude, 1);
SensorCommand<SensorThermo, double> thermoTemp1(&dataQ, &thermo1, "URBAN-Temperature", &SensorThermo::getTemp, 5);

// array definitions: update counts if you make changes here
Sensor *sensors[] = { &gps, &thermo1, &thermo2 };
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &thermoTemp1 };
uint16_t sensor_count = 3;
uint16_t command_count = 3;


// SerialDebugPublishing namespace definitions

/**
 * Publishes a new message to Particle Cloud for urban
**/
void CurrentVehicle::publishMessage() {
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
}

/**
 * Prints urbans's setup message
**/
void CurrentVehicle::setupMessage() {
        DEBUG_SERIAL("TELEMETRY ONLINE - URBAN");
}