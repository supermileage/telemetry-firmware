#include "CanAccessoriesListener.h"

CanAccessoriesListener::CanAccessoriesListener(CanInterface& canInterface, uint16_t id, StatusIds ids)
	: SensorCanBase(canInterface, id) {
	_idArray = ids;
}

void CanAccessoriesListener::begin() {
	_canInterface.addMessageListen(_id, new CanAccessoriesListener::CanAccessoriesMessageParser(this));
}

void CanAccessoriesListener::handle() { }

String CanAccessoriesListener::getHumanName() {
	return "CanAccessoriesListener";
}

String CanAccessoriesListener::getStatus(uint8_t id) {
	uint8_t i = _getCanMessageDataIndex(id);

	if (i >= 8)
		return "NotFound";
	if (_statusMessage.data[i] == 0x0)
		return "false";
	
	return _statusMessage.data[i] - (id << 1) ? "true" : "false";
}

uint8_t CanAccessoriesListener::_getCanMessageDataIndex(uint8_t id) {
	uint8_t i = 0;
	for (uint8_t val : _idArray) {
		if (val == id)
			break;
		++i;
	}
	return i;
}

void CanAccessoriesListener::_updateMessage(uint8_t data) {
	uint8_t id = data >> 1;
	uint8_t i = _getCanMessageDataIndex(id);
	_statusMessage.data[i] = id;
}

// you can probably think of a better pattern for this
void* CanAccessoriesListener::CanAccessoriesMessageParser::execute(void* arg) {
	CanInterface::CanMessage message = *((CanInterface::CanMessage*)arg);
	
	for (uint8_t i = 0; i < message.dataLength; i++) {
		_owner->_updateMessage(message.data[i]);
	}

	DEBUG_SERIAL_LN("Parsing can message in CanAccessoriesListener");

	return (void*)1;
}