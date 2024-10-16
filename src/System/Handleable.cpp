#include "Handleable.h"

Handleable::Handleable() {
	Handler::instance().add(this);
}

Handleable::~Handleable() {
	Handler::instance().remove(this);
}

bool Handleable::isActive() {
	return _isActive;
}

void Handleable::setIsActive(bool active) {
	_isActive = active;
}

bool Handleable::getInitStatus() const {
    return _initialized;
}
