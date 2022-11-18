#include "SensorFcpCellStack.h"
#include "settings.h"

#define NUM_CELLS 18
#define FLOAT_LENGTH 5
#define READ_BUFFER_LENGTH 130

SensorFcpCellStack::SensorFcpCellStack(TelemetrySerial* serial) : _serial(serial) { }

SensorFcpCellStack::~SensorFcpCellStack() { }

String SensorFcpCellStack::getHumanName() {
    return "SensorFcpCellStack";
}

void SensorFcpCellStack::begin() {
    _serial->begin(9600, SERIAL_8N1);
    _cellVoltages.resize(NUM_CELLS);
}

void SensorFcpCellStack::handle() {
    if(millis() < _lastUpdate + STALE_INTERVAL) {
        _valid = true;
    } else {
        _valid = false;
    }
    
    int bytesAvail = _serial->available();
    if (bytesAvail == 0) {
        return;
    }
    

    char buf[READ_BUFFER_LENGTH] = { 0 };
    _serial->readBytes(buf, bytesAvail);

    _buffer.concat(buf);

    if (_containsEndCharacter(buf)) {
        _unpackCellVoltages();
    }
}

String SensorFcpCellStack::getNextCellVoltage(bool& valid) {
    valid = _valid;
    _lastCellVoltageIndex++;
    _lastCellVoltageIndex %= NUM_CELLS;
    return FLOAT_TO_STRING(_cellVoltages[_lastCellVoltageIndex], 2);
}

float SensorFcpCellStack::getCellVoltageByIndex(int index, bool& valid) {
    return 0.0f;
}

bool SensorFcpCellStack::_containsEndCharacter(char* buf) {
    for (int i = 0; buf[i]; i++) {
        if (buf[i] == '\n') {
            return true;
        }
    }
    return false;
}

void SensorFcpCellStack::_unpackCellVoltages() {
    String currentValue = "";
    int j = 0;
    for (uint32_t i = 0; i < _buffer.length(); i++) {
        if (_buffer.c_str()[i] != ' ' && _buffer.c_str()[i] != '\n') {
            currentValue += _buffer[i];
        }
        else {
            _cellVoltages[j++] = currentValue.toFloat();
            currentValue = "";
        }
    }
    _buffer = "";
    _valid = true;
    _lastUpdate = millis();
}
