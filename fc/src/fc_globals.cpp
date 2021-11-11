#include "fc_globals.h"

// sensor definitions
SensorGps gps(GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);

// command definitions
SensorCommand<SensorGps, float> gpsLat(&dataQ, &gps, "lat", &SensorGps::getLatitude, 1);
SensorCommand<SensorGps, float> gpsLong(&dataQ, &gps, "long", &SensorGps::getLongitude, 1);
SensorCommand<SensorGps, float> gpsVertAccel(&dataQ, &gps, "v-accel", &SensorGps::getVerticalAcceleration, 2);
SensorCommand<SensorGps, float> gpsHorAccel(&dataQ, &gps, "h-accel", &SensorGps::getHorizontalAcceleration, 2);
SensorCommand<SensorThermo, double> thermoTemp1(&dataQ, &thermo1, "temp1", &SensorThermo::getTemp, 5);
SensorCommand<SensorThermo, double> thermoTemp2(&dataQ, &thermo2, "temp2", &SensorThermo::getTemp, 5);

// array definitions: update counts if you make changes here
Sensor *sensors[] = { &gps, &thermo1, &thermo2 };
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &gpsVertAccel, &gpsHorAccel, &thermoTemp1, &thermoTemp2 };
uint16_t sensor_count = 3;
uint16_t command_count = 6;


// SerialDebugPublishing namespace definitions

/**
 * Publishes a new message to Particle Cloud
**/
void CurrentVehicle::publishMessage() {
    // Any sensors that are working but not yet packaged for publish
    DEBUG_SERIAL("\nNot in Message: ");
    DEBUG_SERIAL("Current Temperature (Thermo1): " + String(thermo1.getTemp()) + "C");
    DEBUG_SERIAL("Current Temperature (Thermo2): " + String(thermo2.getTemp()) + "C");
    DEBUG_SERIAL("Current Time (UTC): " + Time.timeStr());
    DEBUG_SERIAL();
}

void CurrentVehicle::setupMessage() {
    DEBUG_SERIAL("TELEMETRY ONLINE - FC");
}