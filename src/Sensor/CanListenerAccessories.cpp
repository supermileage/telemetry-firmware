#include "CanListenerAccessories.h"

CanListenerAccessories::CanListenerAccessories(CanInterface* canInterface, uint16_t id, StatusIds ids)
	: CanListener(canInterface, id) {
	_idArray = ids;
	_statusMessage = CAN_MESSAGE_NULL;
	_statusMessage.dataLength = 8;
}

String CanListenerAccessories::getHumanName() {
	return "CanListenerAccessories";
}

int CanListenerAccessories::_getStatus(uint8_t id) {
	uint8_t i = _getCanMessageDataIndex(id);

	if (i >= 8)
		return Unknown;
	if (_statusMessage.data[i] == 0x0)
		return Unknown;
	
	return _statusMessage.data[i] - (id << 1) ? On : Off;
}

int CanListenerAccessories::getStatusHeadlights() {
	return _getStatus(STATUS_HEADLIGHTS);
}

int CanListenerAccessories::getStatusBrakelights() {
	return _getStatus(STATUS_BRAKELIGHTS);
}

int CanListenerAccessories::getStatusHorn() {
	return _getStatus(STATUS_HORN);
}

int CanListenerAccessories::getStatusHazards() {
	return _getStatus(STATUS_HAZARDS);
}

int CanListenerAccessories::getStatusRightSignal() {
	return _getStatus(STATUS_RIGHT_SIGNAL);
}

int CanListenerAccessories::getStatusLeftSignal() {
	return _getStatus(STATUS_LEFT_SIGNAL);
}

int CanListenerAccessories::getStatusWipers() {
	return _getStatus(STATUS_WIPERS);
}

uint8_t CanListenerAccessories::_getCanMessageDataIndex(uint8_t id) {
	uint8_t i = 0;
	for (uint8_t val : _idArray) {
		if (val == id)
			break;
		++i;
	}
	return i;
}

void CanListenerAccessories::_updateMessage(CanMessage message) {
	for (uint8_t j = 0; j < message.dataLength; j++) {
		uint8_t id = message.data[j] >> 1;
		uint8_t index = _getCanMessageDataIndex(id);

		if (index >= _statusMessage.dataLength)
			continue;

		_statusMessage.data[index] = message.data[j];
	}
}