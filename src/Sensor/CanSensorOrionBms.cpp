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

String CanSensorOrionBms::getBatteryVolt(bool& valid) { }

String CanSensorOrionBms::getBatteryCurrent(bool& valid) { }

String CanSensorOrionBms::getMaxVolt(bool& valid) { }
 
String CanSensorOrionBms::getMinVolt(bool& valid) { }

String CanSensorOrionBms::getSoc(bool& valid) { }
 
int CanSensorOrionBms::getStatusBms(bool& valid) { }

String CanSensorOrionBms::getStatusBmsString(bool& valid) { }

int CanSensorOrionBms::getTempBms(bool& valid) { }

void CanSensorOrionBms::setVoltageCallback(void (allback)(float,float)) { }

int CanSensorOrionBms::getFault(bool& valid) { }

void CanSensorOrionBms::restart() { }

void CanSensorOrionBms::update(CanMessage message) {
	switch (message.id) {
		case CAN_ORIONBMS_STATUS:

			break;
		case CAN_ORIONBMS_PACK:
			break;
		case CAN_ORIONBMS_CELL:
			break;
		case CAN_ORIONBMS_TEMP:
			break;
		default:
			break;
	}
}

int16_t parseInt16(uint8_t* buf) {
	return (int16_t)( (*(buf + 1) << 8) | *buf );
}

