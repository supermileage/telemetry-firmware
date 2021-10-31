#include "CommandBase.h"
#include "DataQueue.h"

#ifndef _JSON_LOGGER_H_
#define _JSON_LOGGER_H_

class JsonLogger {
    public:
        JsonLogger();

        JsonLogger(CommandBase **commands, uint8_t numCommands, DataQueue *dataQ, uint16_t interval);

        void log();

        uint16_t getInterval() {
            return _interval;
        }

        unsigned long getLastLog() {
            return _lastLog;
        }

        void setLastLog(unsigned int time) {
            _lastLog = time;
        }

        bool logThisLoop() {
            return _logThisLoop;
        }

        void logThisLoop(bool logThisLoop) {
            _logThisLoop = logThisLoop;
        }

    protected:
        unsigned long _lastLog; 
        DataQueue *_dataQ;
        CommandBase **_logCommands;
        uint16_t _interval;
        uint8_t _numCommands;
        bool _logThisLoop;
};

#endif