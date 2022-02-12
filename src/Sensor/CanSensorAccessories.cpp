#include "CanSensorAccessories.h"

CanSensorAccessories::CanSensorAccessories(CanInterface &canInterface, uint16_t id, StatusIds ids)
	: CanListener(canInterface, id) {
	
	for (uint8_t id : ids)
		_statuses[id] = Unknown;
}

String CanSensorAccessories::getHumanName() {
	return "CanSensorAccessories";
}

int CanSensorAccessories::getStatusHeadlights() {
	return _statuses[STATUS_HEADLIGHTS];
}

int CanSensorAccessories::getStatusBrakelights() {
	return _statuses[STATUS_BRAKELIGHTS];
}

int CanSensorAccessories::getStatusHorn() {
	return _statuses[STATUS_HORN];
}

int CanSensorAccessories::getStatusHazards() {
	return _statuses[STATUS_HAZARDS];
}

int CanSensorAccessories::getStatusRightSignal() {
	return _statuses[STATUS_RIGHT_SIGNAL];
}

int CanSensorAccessories::getStatusLeftSignal() {
	return _statuses[STATUS_LEFT_SIGNAL];
}

int CanSensorAccessories::getStatusWipers() {
	return _statuses[STATUS_WIPERS];
}

void CanSensorAccessories::update(CanMessage message) {
	for (uint8_t i = 0; i < message.dataLength; i++) {
		uint8_t id = message.data[i] >> 1;
		
		if (_statuses.find(id) == _statuses.end())
			continue;

		_statuses[id] = message.data[i] - (id << 1) ? On : Off;
	}
}