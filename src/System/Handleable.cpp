#include "Handleable.h"

Handleable::Handleable() {
    handler.add(this);
}
