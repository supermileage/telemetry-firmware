#include "CanListenerAccessories.h"

CanListenerAccessories::CanListenerAccessories(CanInterface* canInterface, uint16_t id, StatusIds ids)
	: CanListener(canInterface, id) {
	
	for (uint8_t id : ids)
		_statuses[id] = Unknown;
}

String CanListenerAccessories::getHumanName() {
	return "CanListenerAccessories";
}

String CanListenerAccessories::getStatusHeadlights() {
	return String(_statuses[STATUS_HEADLIGHTS]);
}

String CanListenerAccessories::getStatusBrakelights() {
	return String(_statuses[STATUS_BRAKELIGHTS]);
}

String CanListenerAccessories::getStatusHorn() {
	return String(_statuses[STATUS_HORN]);
}

String CanListenerAccessories::getStatusHazards() {
	return String(_statuses[STATUS_HAZARDS]);
}

String CanListenerAccessories::getStatusRightSignal() {
	return String(_statuses[STATUS_RIGHT_SIGNAL]);
}

String CanListenerAccessories::getStatusLeftSignal() {
	return String(_statuses[STATUS_LEFT_SIGNAL]);
}

String CanListenerAccessories::getStatusWipers() {
	return String(_statuses[STATUS_WIPERS]);
}

void CanListenerAccessories::update(CanMessage message) {
	for (uint8_t i = 0; i < message.dataLength; i++) {
		uint8_t id = message.data[i] >> 1;
		
		if (_statuses.find(id) == _statuses.end())
			continue;

		_statuses[id] = message.data[i] - (id << 1) ? On : Off;
	}
}