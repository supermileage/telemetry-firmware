#include "LogThermoCommand.h"

LogThermoCommand::LogThermoCommand(SensorThermo *sensor, DataQueue *dataQ) {
    this->_sensor = sensor;
    this->_dataQ = dataQ;
}

void LogThermoCommand::execute() {
    _dataQ->add("EngineTemp", "SomeThermoData1");
    _dataQ->add("SunTemp", "SomeThermoData2");
    _dataQ->add("SunnyD", "SomeThermoData3");
}