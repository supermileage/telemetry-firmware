#include "Handleable.h"

Handleable::Handleable() {
    handler.add(this);
}

Handleable::~Handleable() {
    handler.remove(this);
}
