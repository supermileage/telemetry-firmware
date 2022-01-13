#include "CanListenerAccessories.h"

CanListenerAccessories::CanListenerAccessories(CanInterface* canInterface, uint16_t id, StatusIds ids)
	: CanListener(canInterface, id) {
	
	for (uint8_t id : ids)
		_statuses[id] = Unknown;
}

String CanListenerAccessories::getHumanName() {
	return "CanListenerAccessories";
}

int CanListenerAccessories::getStatusHeadlights() {
	return _statuses[STATUS_HEADLIGHTS];
}

int CanListenerAccessories::getStatusBrakelights() {
	return _statuses[STATUS_BRAKELIGHTS];
}

int CanListenerAccessories::getStatusHorn() {
	return _statuses[STATUS_HORN];
}

int CanListenerAccessories::getStatusHazards() {
	return _statuses[STATUS_HAZARDS];
}

int CanListenerAccessories::getStatusRightSignal() {
	return _statuses[STATUS_RIGHT_SIGNAL];
}

int CanListenerAccessories::getStatusLeftSignal() {
	return _statuses[STATUS_LEFT_SIGNAL];
}

int CanListenerAccessories::getStatusWipers() {
	return _statuses[STATUS_WIPERS];
}

void CanListenerAccessories::update(CanMessage message) {
	for (uint8_t i = 0; i < message.dataLength; i++) {
		uint8_t id = message.data[i] >> 1;
		
		if (_statuses.find(id) == _statuses.end())
			continue;

		_statuses[id] = message.data[i] - (id << 1) ? On : Off;
	}
}