#include "Particle.h"
#include "settings.h"

#include "IntervalLogger.h"
#include "DataQueue.h"
#include "Command.h"

IntervalLogger::IntervalLogger() { }

IntervalLogger::IntervalLogger(Command **commands, uint8_t numCommands, uint16_t interval) {
    _commands = commands;
    _numCommands = numCommands;
    _interval = interval;
    _lastLog = 0;
    _logThisLoop = false;
}

void IntervalLogger::log(DataQueue *dataQ) {
    for (uint8_t i = 0; i < _numCommands; i++) {
        _commands[i]->execute(dataQ);
    }
}

uint16_t IntervalLogger::getInterval() {
    return _interval;
}

unsigned long IntervalLogger::getLastLog() {
    return _lastLog;
}

void IntervalLogger::setLastLog(unsigned int time) {
    _lastLog = time;
}

bool IntervalLogger::logThisLoop() {
    return _logThisLoop;
}

void IntervalLogger::logThisLoop(bool logThisLoop) {
    _logThisLoop = logThisLoop;
}