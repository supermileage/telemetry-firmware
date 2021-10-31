#include "Particle.h"
#include "settings.h"

#include "JsonLogger.h"
#include "DataQueue.h"
#include "CommandBase.h"

JsonLogger::JsonLogger() { }

JsonLogger::JsonLogger(CommandBase **commands, uint8_t numCommands, DataQueue *dataQ, uint16_t interval) {
    _logCommands = commands;
    _dataQ = dataQ;
    _numCommands = numCommands;
    _interval = interval * 1000;
    _lastLog = 0;
    _logThisLoop = false;
}

void JsonLogger::log() {
    for (uint8_t i = 0; i < _numCommands; i++) {
        _logCommands[i]->execute(_dataQ);
    }
}