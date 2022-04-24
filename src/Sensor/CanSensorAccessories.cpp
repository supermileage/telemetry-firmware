#include "CanSensorAccessories.h"

#define ACC_STATUS_HEADLIGHTS		0x0
#define ACC_STATUS_BRAKELIGHTS		0x1
#define ACC_STATUS_HORN				0x2
#define ACC_STATUS_HAZARDS			0x3
#define ACC_STATUS_RIGHT_SIGNAL		0x4
#define ACC_STATUS_LEFT_SIGNAL		0x5
#define ACC_STATUS_WIPERS			0x6

const uint8_t STATUS_IDS[] {
    ACC_STATUS_HEADLIGHTS,
    ACC_STATUS_BRAKELIGHTS,
    ACC_STATUS_HORN,
    ACC_STATUS_HAZARDS,
    ACC_STATUS_RIGHT_SIGNAL,
    ACC_STATUS_LEFT_SIGNAL,
	ACC_STATUS_WIPERS
};

CanSensorAccessories::CanSensorAccessories(CanInterface &canInterface, uint16_t id)
	: CanListener(canInterface, id) { }

void CanSensorAccessories::begin() {
	CanListener::begin();

	for (uint8_t id : STATUS_IDS)
		_statuses[id] = CanSensorAccessories::StatusProperty { 0, Unknown };
}

String CanSensorAccessories::getHumanName() {
	return "CanSensorAccessories";
}

int CanSensorAccessories::getStatusHeadlights(bool& valid) {
	return _getStatus(ACC_STATUS_HEADLIGHTS, valid);
}

int CanSensorAccessories::getStatusBrakelights(bool& valid) {
	return _getStatus(ACC_STATUS_BRAKELIGHTS, valid);
}

int CanSensorAccessories::getStatusHorn(bool& valid) {
	return _getStatus(ACC_STATUS_HORN, valid);
}

int CanSensorAccessories::getStatusHazards(bool& valid) {
	return _getStatus(ACC_STATUS_HAZARDS, valid);
}

int CanSensorAccessories::getStatusRightSignal(bool& valid) {
	return _getStatus(ACC_STATUS_RIGHT_SIGNAL, valid);
}

int CanSensorAccessories::getStatusLeftSignal(bool& valid) {
	return _getStatus(ACC_STATUS_LEFT_SIGNAL, valid);
}

int CanSensorAccessories::getStatusWipers(bool& valid) {
	return _getStatus(ACC_STATUS_WIPERS, valid);
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