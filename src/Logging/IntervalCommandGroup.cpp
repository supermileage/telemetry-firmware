#include "IntervalCommandGroup.h"


IntervalCommandGroup::IntervalCommandGroup() { }

IntervalCommandGroup::IntervalCommandGroup(Command **commands, uint8_t numCommands, uint16_t interval) {
    _commands = commands;
    _numCommands = numCommands;
    _interval = interval;
    _lastExecution = 0;
    _executeThisLoop = false;
}

IntervalCommandGroup::~IntervalCommandGroup() {
    for (uint16_t i = 0; i < _numCommands; i++) { 
        delete _commands[i];
    }
    delete[] _commands;
}

void IntervalCommandGroup::executeCommands(CommandArgs args) {
    for (uint8_t i = 0; i < _numCommands; i++) {
<<<<<<< HEAD
        _commands[i]->execute(nullptr);
=======
        _commands[i]->execute(args);
>>>>>>> develop
    }
}

uint16_t IntervalCommandGroup::getInterval() {
    return _interval;
}

unsigned long IntervalCommandGroup::getLastExecution() {
    return _lastExecution;
}

void IntervalCommandGroup::setLastExecution(unsigned int time) {
    _lastExecution = time;
}

bool IntervalCommandGroup::executeThisLoop() {
    return _executeThisLoop;
}

void IntervalCommandGroup::executeThisLoop(bool executeThisLoop) {
    _executeThisLoop = executeThisLoop;
}