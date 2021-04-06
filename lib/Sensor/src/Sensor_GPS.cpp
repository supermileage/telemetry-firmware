#include "Sensor_GPS.h"

void Sensor_GPS::begin() {
    Wire.begin();
    GPS.begin();
}

void Sensor_GPS::handle() {
    GPS.checkUblox();
}