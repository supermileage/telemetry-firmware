#include "CanListenerAccessories.h"

CanListenerAccessories::CanListenerAccessories(CanInterface& canInterface, uint16_t id, StatusIds ids)
	: CanListener(canInterface, id) {
	_idArray = ids;
	_statusMessage.dataLength = 8;
}

String CanListenerAccessories::getHumanName() {
	return "CanListenerAccessories";
}

String CanListenerAccessories::_getStatus(uint8_t id) {
	uint8_t i = _getCanMessageDataIndex(id);

	if (i >= 8)
		return "NotFound";
	if (_statusMessage.data[i] == 0x0)
		return "false";
	
	return _statusMessage.data[i] - (id << 1) ? "true" : "false";
}

String CanListenerAccessories::getStatusHeadlights() {
	return _getStatus(STATUS_HEADLIGHTS);
}

String CanListenerAccessories::getStatusBrakelights() {
	return _getStatus(STATUS_BRAKELIGHTS);
}

String CanListenerAccessories::getStatusHorn() {
	return _getStatus(STATUS_HORN);
}

String CanListenerAccessories::getStatusHazards() {
	return _getStatus(STATUS_HAZARDS);
}

String CanListenerAccessories::getStatusRightSignal() {
	return _getStatus(STATUS_RIGHT_SIGNAL);
}

String CanListenerAccessories::getStatusLeftSignal() {
	return _getStatus(STATUS_LEFT_SIGNAL);
}

String CanListenerAccessories::getStatusWipers() {
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