#include "SensorFcpCellStack.h"
#include "settings.h"

// #define DEBUG_FCP_SERIAL

#define FC_BAUD 9600

#define NUM_CELLS 17
#define NUM_HEADERS 6
#define PACKET_LENGTH NUM_CELLS * 2 + NUM_HEADERS

#define FC_HEADER_0 0x7
#define FC_HEADER_1 0xD
#define FC_HEADER_2 0x11
#define FC_HEADER_3 0x17
#define FC_HEADER_4 0x1D
#define FC_HEADER_5 0x1F

SensorFcpCellStack::SensorFcpCellStack(TelemetrySerial* serial) : _serial(serial) { }

SensorFcpCellStack::~SensorFcpCellStack() { }

String SensorFcpCellStack::getHumanName() {
    return "SensorFcpCellStack";
}

void SensorFcpCellStack::begin() {
    _serial->begin(FC_BAUD, SERIAL_8N1);
    _cellVoltages.resize(NUM_CELLS);
}

void SensorFcpCellStack::handle() {
    if(millis() < _lastUpdate + STALE_INTERVAL) {
        _valid = true;
    } else {
        _valid = false;
    }
    
    int bytesAvail = _serial->available();
    if (bytesAvail < PACKET_LENGTH) {
        return;
    }
    
    uint8_t buf[PACKET_LENGTH] = { 0 };
	_serial->readBytes((char*)buf, PACKET_LENGTH);

	if (buf[0] != FC_HEADER_0 || buf[1] != FC_HEADER_1 || buf[2] != FC_HEADER_2 ||
		buf[3] != FC_HEADER_3 || buf[4] != FC_HEADER_4 || buf[5] != FC_HEADER_5) {
			_flushSerial();
			return;
	}

	_unpackCellVoltages(buf);
}

int SensorFcpCellStack::getNumFuelCells() {
	return NUM_CELLS;
}

String SensorFcpCellStack::getNextCellVoltage(bool& valid) {
    valid = _valid;
    _lastCellVoltageIndex++;
    _lastCellVoltageIndex %= NUM_CELLS;
    return FLOAT_TO_STRING(_cellVoltages[_lastCellVoltageIndex], 2);
}

float SensorFcpCellStack::getCellVoltageByIndex(int index) {
	if (index < NUM_CELLS)
    	return _cellVoltages[index];
	else
		return 0.0f;
}

void SensorFcpCellStack::_unpackCellVoltages(uint8_t* buf) {
	int j = 0;
	for (int i = NUM_HEADERS; i < PACKET_LENGTH; i+=2) {
		int16_t val = (buf[i] << 8) | buf[i+1];
		_cellVoltages[j++] = (float)val / 1000.0f;

		#ifdef DEBUG_FCP_SERIAL
		DEBUG_SERIAL_LN("UNPACKED VALUE: " + FLOAT_TO_STRING(_cellVoltages[j-1], 1));
		#endif
	}
    _lastUpdate = millis();
}

void SensorFcpCellStack::_flushSerial() {
	while (_serial->available()) {
		_serial->read();
	}
}
