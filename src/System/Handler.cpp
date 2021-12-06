#include "Handler.h"


void Handler::add(Handleable* handleable) {
    _handleables.push_back(handleable);
}

void Handler::begin() {
    for(Handleable *h : _handleables) {
        h->begin();
    }
}

void Handler::handle() {
    for(Handleable *h : _handleables) {
        h->handle();
    }
}

    