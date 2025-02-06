#include "SensorFcpControl.h"
#include "fcp-common.h"
#include "settings.h"

// #define DEBUG_FCP_CONTROL

#ifdef DEBUG_FCP_CONTROL
#define FC_DEBUG_INTERVAL 100
uint32_t sensor_fcp_last_debug_output = 0;
#endif

#define FC_PACKET_LENGTH FC_NUM_HEADERS + FC_NUM_CELLS * 2

const int32_t SensorFcpControl::PacketSize = FC_PACKET_LENGTH;

SensorFcpControl::SensorFcpControl(TelemetrySerial* serial) : _serial(serial) { }

SensorFcpControl::~SensorFcpControl() { }

String SensorFcpControl::getHumanName() {
    return "FCP Control";
}

void SensorFcpControl::begin() {
    _serial->begin(FC_BAUD, SERIAL_8N1);
    _cellVoltages.resize(FC_NUM_CELLS);
}

void SensorFcpControl::handle() {
    if(millis() < _lastUpdate + STALE_INTERVAL) {
        _valid = true;
    } else {
        _valid = false;
    }
    
    int bytesAvail = _serial->available();
    if (bytesAvail < FC_PACKET_LENGTH) {
		#ifdef DEBUG_FCP_CONTROL
		if (bytesAvail > 0 && millis() >= sensor_fcp_last_debug_output + FC_DEBUG_INTERVAL) {
			sensor_fcp_last_debug_output = millis();
			DEBUG_SERIAL_F("Received %d bytes from Fcp Control\n", _serial->available());
		}
		#endif
        return;
    }
    
    uint8_t buf[FC_PACKET_LENGTH] = { 0 };
	_serial->readBytes((char*)buf, FC_PACKET_LENGTH);

	#ifdef DEBUG_FCP_CONTROL
		DEBUG_SERIAL_LN("-----------------------------");
		DEBUG_SERIAL("SensorFcpControl Received Message - Header:");
		for (int i = 0; i < FC_NUM_HEADERS; i++) {
			DEBUG_SERIAL_F(" 0x%x", buf[i]);
		}
		DEBUG_SERIAL("\n");
		DEBUG_SERIAL("Data: ");
		for (int i = FC_NUM_HEADERS; i < FC_PACKET_LENGTH; i++) { // print the data
			DEBUG_SERIAL_F("0x%x ", buf[i]);
		}
		DEBUG_SERIAL_LN();
	#endif

	if (buf[0] != FC_HEADER_0 || buf[1] != FC_HEADER_1 || buf[2] != FC_HEADER_2 ||
		buf[3] != FC_HEADER_3 || buf[4] != FC_HEADER_4 || buf[5] != FC_HEADER_5) {
			#ifdef DEBUG_FCP_CONTROL
			DEBUG_SERIAL_LN("FcpControl Header Incorrect -- flushing data");
			#endif
			_flushSerial();
			return;
	}
	
	_unpackCellVoltages(buf);
}

int SensorFcpControl::getNumFuelCells() {
	return FC_NUM_CELLS;
}

String SensorFcpControl::getNextCellVoltage(bool& valid) {
    valid = _valid;
    _lastCellVoltageIndex++;
    _lastCellVoltageIndex %= FC_NUM_CELLS;
    return FLOAT_TO_STRING(_cellVoltages[_lastCellVoltageIndex], 2);
}

float SensorFcpControl::getCellVoltageByIndex(int index, bool& valid) {
	valid = _valid;
	if (index < FC_NUM_CELLS)
    	return _cellVoltages[index];
	else
		return 0.0f;
}

String SensorFcpControl::getStackVoltage(bool& valid) {
    valid = _valid;
    float sum = 0;
    for (float cell : _cellVoltages) {
        sum += cell;
    }
    return FLOAT_TO_STRING(sum, 2);
}

void SensorFcpControl::_unpackCellVoltages(uint8_t* buf) {
	int j = 0;
	for (int i = FC_NUM_HEADERS; i < FC_PACKET_LENGTH; i+=2) {
		int16_t val = (buf[i] << 8) | buf[i+1];
		_cellVoltages[j++] = (float)val / 1000.0f;

		#ifdef DEBUG_FCP_CONTROL
		DEBUG_SERIAL_LN("UNPACKED VALUE: " + FLOAT_TO_STRING(_cellVoltages[j-1], 1));
		#endif
	}
	_valid = true;
    _lastUpdate = millis();
}

void SensorFcpControl::_flushSerial() {
	while (_serial->available()) {
		_serial->read();
	}
}
