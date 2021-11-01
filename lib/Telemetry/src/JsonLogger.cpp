#include "Particle.h"
#include "settings.h"

#include "JsonLogger.h"
#include "DataQueue.h"
#include "CommandBase.h"

JsonLogger::JsonLogger() { }

JsonLogger::JsonLogger(CommandBase **commands, uint8_t numCommands, uint16_t interval) {
    _logCommands = commands;
    _numCommands = numCommands;
    _interval = interval * 1000;
    _lastLog = 0;
    _logThisLoop = false;
}

void JsonLogger::log(DataQueue *dataQ) {
    for (uint8_t i = 0; i < _numCommands; i++) {
        _logCommands[i]->execute(dataQ);
    }
}