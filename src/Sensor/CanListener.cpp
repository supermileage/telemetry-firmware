#include "CanListener.h"

CanListener::CanListener(CanInterface &canInterface) : _canInterface(canInterface) { }

CanListener::CanListener(CanInterface &canInterface, uint16_t id) : _canInterface(canInterface), _id(id) { }

void CanListener::begin() {
	_canInterface.addMessageListen(_id, new CanListener::CanListenerDelegate(this));
}

void CanListener::CanListenerDelegate::execute(CommandArgs args) {
	CanMessage message = *((CanMessage*)args);
	_owner->update(message);
}