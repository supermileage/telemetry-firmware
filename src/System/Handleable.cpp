#include "Handleable.h"

// Static Object Declaration
Handler* Handleable::_handlerInstance;

// Static Method
Handler& Handleable::handlerInstance() {
    return *_handlerInstance;
}

Handleable::Handleable() {

    if(!_handlerInstance) {
        _handlerInstance = new Handler();
    }

    _handlerInstance->add(this);
}

Handleable::~Handleable() {
    _handlerInstance->remove(this);
}
