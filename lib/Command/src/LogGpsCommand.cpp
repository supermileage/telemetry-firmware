#include "LogGpsCommand.h"

LogGpsCommand::LogGpsCommand(SensorGps *sensor, DataQueue *dataQ) {
    this->_sensor = sensor;
    this->_dataQ = dataQ;
}

void LogGpsCommand::execute() {
    _dataQ->add("Location", "SomeValue1");
    _dataQ->add("Speed", "SomeValue2");
}