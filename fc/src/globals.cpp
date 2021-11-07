#include "globals.h"

DataQueue dataQ("fc");

SensorGps gps(GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);
SensorThermo thermo2(&SPI, A4, THERMO_UPDATE_INTERVAL_MS);

LogCommand<SensorGps, float> gpsLat(&dataQ, &gps, "lat", &SensorGps::getLatitude, 1);
LogCommand<SensorGps, float> gpsLong(&dataQ, &gps, "long", &SensorGps::getLongitude, 1);
LogCommand<SensorGps, float> gpsVertAccel(&dataQ, &gps, "v-accel", &SensorGps::getVerticalAcceleration, 2);
LogCommand<SensorGps, float> gpsHorAccel(&dataQ, &gps, "h-accel", &SensorGps::getHorizontalAcceleration, 2);
LogCommand<SensorThermo, double> thermoTemp1(&dataQ, &thermo1, "temp1", &SensorThermo::getTemp, 5);
LogCommand<SensorThermo, double> thermoTemp2(&dataQ, &thermo2, "temp2", &SensorThermo::getTemp, 5);

Sensor *sensors[NUM_SENSORS] = {&gps, &thermo1, &thermo2};
IntervalCommand *commands[NUM_LOG_COMMANDS] = { &gpsLat, &gpsLong, &gpsVertAccel, &gpsHorAccel, &thermoTemp1, &thermoTemp2 };