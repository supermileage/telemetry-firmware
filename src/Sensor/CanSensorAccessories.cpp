#include "CanSensorAccessories.h"

const uint8_t CanSensorAccessories::StatusIdHeadlights = 0x0;
const uint8_t CanSensorAccessories::StatusIdBrakelights = 0x1;
const uint8_t CanSensorAccessories::StatusIdHorn = 0x2;
const uint8_t CanSensorAccessories::StatusIdHazards = 0x3;
const uint8_t CanSensorAccessories::StatusIdRightSignal = 0x4;
const uint8_t CanSensorAccessories::StatusIdLeftSignal = 0x5;
const uint8_t CanSensorAccessories::StatusIdWipers = 0x6;

const uint8_t statusIds[] {
    CanSensorAccessories::StatusIdHeadlights,
    CanSensorAccessories::StatusIdBrakelights,
    CanSensorAccessories::StatusIdHorn,
    CanSensorAccessories::StatusIdHazards,
    CanSensorAccessories::StatusIdRightSignal,
    CanSensorAccessories::StatusIdLeftSignal,
	CanSensorAccessories::StatusIdWipers
};

CanSensorAccessories::CanSensorAccessories(CanInterface &canInterface, uint16_t id)
	: CanListener(canInterface, id) { }

void CanSensorAccessories::begin() {
	CanListener::begin();

	for (uint8_t id : statusIds)
		_statuses[id] = CanSensorAccessories::StatusProperty { 0, Unknown };
}

String CanSensorAccessories::getHumanName() {
	return "CanSensorAccessories";
}

int CanSensorAccessories::getStatusHeadlights(bool& valid) {
	return _getStatus(CanSensorAccessories::StatusIdHeadlights, valid);
}

int CanSensorAccessories::getStatusBrakelights(bool& valid) {
	return _getStatus(CanSensorAccessories::StatusIdBrakelights, valid);
}

int CanSensorAccessories::getStatusHorn(bool& valid) {
	return _getStatus(CanSensorAccessories::StatusIdHorn, valid);
}

int CanSensorAccessories::getStatusHazards(bool& valid) {
	return _getStatus(CanSensorAccessories::StatusIdHazards, valid);
}

int CanSensorAccessories::getStatusRightSignal(bool& valid) {
	return _getStatus(CanSensorAccessories::StatusIdRightSignal, valid);
}

int CanSensorAccessories::getStatusLeftSignal(bool& valid) {
	return _getStatus(CanSensorAccessories::StatusIdLeftSignal, valid);
}

int CanSensorAccessories::getStatusWipers(bool& valid) {
	return _getStatus(CanSensorAccessories::StatusIdWipers, valid);
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
	valid = status.value != Unknown && (millis() - status.lastUpdated) < STALE_INTERVAL;
	return status.value;
}