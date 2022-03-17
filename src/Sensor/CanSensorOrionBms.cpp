#include "CanSensorOrionBms.h"

CanSensorOrionBms::CanSensorOrionBms(CanInterface& canInterface) : CanSensorBms(canInterface) { }

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

int CanSensorOrionBms::getFault(bool& valid) { }

void CanSensorOrionBms::restart() { }

void CanSensorOrionBms::update(CanMessage message) {
	switch (message.id) {
		case CAN_ORIONBMS_STATUS:
			
			break;
		case CAN_ORIONBMS_PACK:
			_batteryVoltage = (float)parseInt16(message.data) / 10.0f;
			_batteryCurrent = (float)parseInt16(message.data + 2) / 10.0f;
			_soc = (float)message.data[4] / 2.0f;
			break;
		case CAN_ORIONBMS_CELL:
			// TODO: Add unify this with TinyBms
			_cellVoltageMin = (float)parseInt16(message.data) / 1000.0f;
			_cellVoltageMax = (float)parseInt16(message.data + 2) / 1000.0f;
			_cellVoltageAvg = (float)parseInt16(message.data + 4) / 1000.0f;
			break;
		case CAN_ORIONBMS_TEMP:
			_batteryTemp1 = (int)message.data[0];
			_batteryTemp2 = (int)message.data[1];
			_batteryTempAvg = (int)message.data[2];
			_tempBms = (int)message.data[3];
			break;
		default:
			break;
	}
}

float parseInt16(uint8_t* buf) {
	return (float)( (*(buf + 1) << 8) | *buf );
}

