#include "CanSensorAccessories.h"

#define VALID_INTERVAL 2000;

CanSensorAccessories::CanSensorAccessories(CanInterface &canInterface, uint16_t id, StatusIds ids)
	: CanListener(canInterface, id) {
	
	for (uint8_t id : ids)
		_statuses[id] = CanSensorAccessories::StatusProperty { 0, Unknown };
}

String CanSensorAccessories::getHumanName() {
	return "CanSensorAccessories";
}

int CanSensorAccessories::getStatusHeadlights(bool& valid) {
	return _getStatus(STATUS_HEADLIGHTS, valid);
}

int CanSensorAccessories::getStatusBrakelights(bool& valid) {
	return _getStatus(STATUS_BRAKELIGHTS, valid);
}

int CanSensorAccessories::getStatusHorn(bool& valid) {
	return _getStatus(STATUS_HORN, valid);
}

int CanSensorAccessories::getStatusHazards(bool& valid) {
	return _getStatus(STATUS_HAZARDS, valid);
}

int CanSensorAccessories::getStatusRightSignal(bool& valid) {
	return _getStatus(STATUS_RIGHT_SIGNAL, valid);
}

int CanSensorAccessories::getStatusLeftSignal(bool& valid) {
	return _getStatus(STATUS_LEFT_SIGNAL, valid);
}

int CanSensorAccessories::getStatusWipers(bool& valid) {
	return _getStatus(STATUS_WIPERS, valid);
}

void CanSensorAccessories::update(CanMessage message) {
	unsigned long time = millis();

	for (uint8_t i = 0; i < message.dataLength; i++) {
		uint8_t id = message.data[i] >> 1;
		
		if (_statuses.find(id) == _statuses.end())
			continue;

		_statuses[id] = CanSensorAccessories::StatusProperty { time, message.data[i] - (id << 1) ? On : Off };
	}
}

int CanSensorAccessories::_getStatus(uint8_t id, bool& valid) {
	CanSensorAccessories::StatusProperty status = _statuses[id];
	valid = status.value != Unknown && (millis() - status.lastUpdated) <= VALID_INTERVAL;
	return status.value;
}