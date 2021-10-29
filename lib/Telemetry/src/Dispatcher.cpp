#include "Dispatcher.h"
#include "DataQueue.h"

#include "settings.h"
#include "Particle.h"


Dispatcher::Dispatcher(Command *commands, DataQueue *dataQ, uint8_t len, uint16_t interval) {
    _commands = commands;
    _dataQ = dataQ;
    _numCommands = len;
    _interval = interval * 1000;
    _lastDispatch = 0;
}

void Dispatcher::run(uint32_t time) {
    if (time >= (_lastDispatch + _interval)) {
        dispatch();
        _lastDispatch = time;
    }
}

void Dispatcher::dispatch() {
    

    _dataQ->wrapStart();

    for (uint8_t i = 0; i < _numCommands; i++) {
        _commands[i].execute();
    }

    _dataQ->wrapEnd();
}