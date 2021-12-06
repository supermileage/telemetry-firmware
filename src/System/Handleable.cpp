#include "Handleable.h"

Handleable::Handleable() {
    Serial.println("HANDLEABLE ADDED!");
    handler.add(this);
}

Handleable::~Handleable() {}
