#include "LogCanCommand.h"

LogCanCommand::LogCanCommand(SensorCan *sensor, DataQueue *dataQ) {
    this->_sensor = sensor;
    this->_dataQ = dataQ;
}

void LogCanCommand::execute() {
    _dataQ->add("CanData", (int)_sensor->getData(0));
}