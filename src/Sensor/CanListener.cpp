#include "CanListener.h"

void CanListener::begin() {
	_canInterface->addMessageListen(_id, new CanListener::CanListenerDelegate(this));
}

void CanListener::CanListenerDelegate::execute(CommandArgs args) {
	CanMessage message = *((CanMessage*)args);
	_owner->update(message);
}