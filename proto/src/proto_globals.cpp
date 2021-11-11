#include "proto_globals.h"

// sensor definitions
SensorGps gps(GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);
SensorEcu ecu(&Serial1);

// command definitions
SensorCommand<SensorEcu, float> ecuEct(&dataQ, &ecu, "PROTO-ECT", &SensorEcu::getECT, 5);
SensorCommand<SensorEcu, float> ecuIat(&dataQ, &ecu, "PROTO-IAT", &SensorEcu::getIAT, 5);
SensorCommand<SensorEcu, float> ecuRpm(&dataQ, &ecu, "PROTO-RPM", &SensorEcu::getRPM, 5);
SensorCommand<SensorEcu, float> ecuUbAdc(&dataQ, &ecu, "PROTO-UBADC", &SensorEcu::getUbAdc, 5);
SensorCommand<SensorEcu, float> ecu02S(&dataQ, &ecu, "PROTO-O2S", &SensorEcu::getO2S, 5);
SensorCommand<SensorEcu, float> ecuSpark(&dataQ, &ecu, "PROTO-SPARK", &SensorEcu::getSpark, 5);
SensorCommand<SensorGps, float> gpsLat(&dataQ, &gps, "PROTO-Latitude", &SensorGps::getLatitude, 1);
SensorCommand<SensorGps, float> gpsHvel(&dataQ, &gps, "PROTO-Speed", &SensorGps::getHorizontalSpeed, 1);

// array definitions: update counts if you make changes here
Sensor *sensors[] = {&ecu, &gps, &thermo1, &thermo2};
IntervalCommand *commands[] = { &ecuEct, &ecuIat, &ecuRpm, &ecuUbAdc, &ecu02S, &ecuSpark, &gpsLat, &gpsHvel };
uint16_t sensor_count = 4;
uint16_t command_count = 8;


// SerialDebugPublishing namespace definitions

/**
 * Publishes a new message to Particle Cloud for proto
**/
void CurrentVehicle::publishMessage() {
    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("\nNot in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermo1.getTemp()) + "C");
    DEBUG_SERIAL("Current Temperature (Thermo2): " + String(thermo2.getTemp()) + "C");
    DEBUG_SERIAL("Current Time (UTC): " + Time.timeStr());
    DEBUG_SERIAL();
}

/**
 * Prints proto's setup message
**/
void CurrentVehicle::setupMessage() {
    DEBUG_SERIAL("TELEMETRY ONLINE - PROTO");
}