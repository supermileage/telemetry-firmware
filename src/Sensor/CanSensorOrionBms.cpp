#include "CanSensorOrionBms.h"

const uint16_t VALIDATION_IDS[] {
	CAN_ORIONBMS_STATUS,
 	CAN_ORIONBMS_PACK,
	CAN_ORIONBMS_CELL,
	CAN_ORIONBMS_TEMP,
};

CanSensorOrionBms::CanSensorOrionBms(CanInterface& canInterface) : CanSensorBms(canInterface) {
	for (uint16_t id : VALIDATION_IDS) {
		_validationMap[id] = 0;
	}
}

CanSensorOrionBms::~CanSensorOrionBms() { }

void CanSensorOrionBms::begin() {
	CanListenerDelegate* orionDelegate = new CanListener::CanListenerDelegate(this);
	_canInterface.addMessageListen(CAN_ORIONBMS_STATUS, orionDelegate);
	_canInterface.addMessageListen(CAN_ORIONBMS_PACK, orionDelegate);
	_canInterface.addMessageListen(CAN_ORIONBMS_CELL, orionDelegate);
	_canInterface.addMessageListen(CAN_ORIONBMS_TEMP, orionDelegate);
}

void CanSensorOrionBms::handle() { }

String CanSensorOrionBms::getHumanName() {
	return "CanSensorOrionBms";
}

void CanSensorOrionBms::restart() { }

void CanSensorOrionBms::update(CanMessage message) {
	uint64_t time = millis();

	switch (message.id) {
		case CAN_ORIONBMS_STATUS:
			_bmsStatus = message.data[0] & 0x1 ? DischargeEnabled : Unknown;
			_bmsStatus = message.data[0] & 0x2 ? ChargeEnabled : Unknown;
			_fault = _parseFault(message);
			_validationMap[CAN_ORIONBMS_STATUS] = time;
			break;
		case CAN_ORIONBMS_PACK:
			_batteryVoltage = (float)_parseInt16(message.data) / 10.0f;
			_batteryCurrent = (float)_parseInt16(message.data + 2) / 10.0f;
			_soc = (float)message.data[4] / 2.0f;
			_voltageCallback(_soc, _batteryVoltage);
			_validationMap[CAN_ORIONBMS_PACK] = time;
			break;
		case CAN_ORIONBMS_CELL:
			// TODO: Add unify this with TinyBms
			_cellVoltageMin = (float)_parseInt16(message.data) / 1000.0f;
			_cellVoltageMax = (float)_parseInt16(message.data + 2) / 1000.0f;
			_cellVoltageAvg = (float)_parseInt16(message.data + 4) / 1000.0f;
			_validationMap[CAN_ORIONBMS_CELL] = time;
			break;
		case CAN_ORIONBMS_TEMP:
			_batteryTempMax = (int)message.data[0];
			_batteryTempMin = (int)message.data[1];
			_batteryTempAvg = (int)message.data[2];
			_tempBms = (int)message.data[3];
			_validationMap[CAN_ORIONBMS_TEMP] = time;
			break;
		default:
			break;
	}
}

String CanSensorOrionBms::getBatteryVolt(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_PACK);
    return FLOAT_TO_STRING(_batteryVoltage, 1);
}

String CanSensorOrionBms::getBatteryCurrent(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_PACK);
    return FLOAT_TO_STRING(_batteryCurrent, 1);
}

String CanSensorOrionBms::getMaxVolt(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_CELL);
    return FLOAT_TO_STRING(_cellVoltageMax, 3);
}

String CanSensorOrionBms::getMinVolt(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_CELL);
    return FLOAT_TO_STRING(_cellVoltageMin, 3);
}

String CanSensorOrionBms::getAvgVolt(bool& valid) {
	valid  = _validate(CAN_ORIONBMS_CELL);
    return FLOAT_TO_STRING(_cellVoltageAvg, 3);
}

String CanSensorOrionBms::getSoc(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_PACK);
    return FLOAT_TO_STRING(_soc, 1); 
}

int CanSensorOrionBms::getTempBms(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_TEMP);
    return _tempBms;
}

int CanSensorOrionBms::getMaxBatteryTemp(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_TEMP);
    return _batteryTempMax;
}

int CanSensorOrionBms::getMinBatteryTemp(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_TEMP);
    return _batteryTempMin;
}

int CanSensorOrionBms::getAvgBatteryTemp(bool& valid) {
	valid = _validate(CAN_ORIONBMS_TEMP);
	return _batteryTempAvg;
}


int CanSensorOrionBms::getFault(bool& valid) {
	valid = _validate(CAN_ORIONBMS_STATUS);
	return _fault;
}

int CanSensorOrionBms::getStatusBms(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_STATUS);
    return _bmsStatus;
}

String CanSensorOrionBms::getStatusBmsString(bool& valid) {
    valid  = _validate(CAN_ORIONBMS_STATUS);
    return String(BMS_STATUS_STRINGS[_bmsStatus]);
}

int16_t CanSensorOrionBms::_parseInt16(uint8_t* buf) {
	return (float)( *buf << 8 | *(buf + 1) );
}

// Faults are prioritized from left to right, most significant bit to least significant
// and numbered from 1 (highest) to 19 (lowest).  Please refer to notion for more info
int CanSensorOrionBms::_parseFault(CanMessage message) {
	int fault = 1;
	for (int i = 1; i < 4; i++) {
		uint8_t currentByte = message.data[i];

		int bitCount = 8;
		if (i == 3)
			bitCount = 3;

		for (int j = 0; j < bitCount; j++) {
			if (currentByte & (1 << (bitCount - j - 1))) {
				return fault > 5 ? fault + 2 : fault;
			}
			fault++;
		}
	}
	return 0;
}