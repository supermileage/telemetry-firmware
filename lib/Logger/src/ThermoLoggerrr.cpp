#include "ThermoLogger.h"

#include "settings.h"

ThermoLogger::ThermoLogger(SensorThermo *sensor) {
    this->_sensor = sensor;
}