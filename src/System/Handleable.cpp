#include "Handleable.h"

Handleable::Handleable() {
    Handler::instance().add(this);
}

Handleable::~Handleable() {
    Handler::instance().remove(this);
}
