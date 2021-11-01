#include "Particle.h"
#include "DataQueue.h"
#include "JsonLogger.h"

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

class Dispatcher {
    public:
        Dispatcher(JsonLogger **loggers, uint16_t numLoggers, DataQueue *dataQ) {
            _loggers = loggers;
            _numLoggers = numLoggers;
            _dataQ = dataQ;
            _logThisLoop = false;
        }

        void run(unsigned long time) {

            // check if it's time to log any data from any of the loggers
            for (uint16_t i = 0; i < _numLoggers; i++) {
                if ((_loggers[i]->getLastLog() + _loggers[i]->getInterval()) <= time) {
                    _loggers[i]->setLastLog(time);
                    _logThisLoop = true;
                    _loggers[i]->logThisLoop(true);
                }   
            }

            // call log on loggers and reset logThisLoop bool
            if (_logThisLoop) {
                _dataQ->wrapStart();
                for (uint16_t i = 0; i < _numLoggers; i++) {
                    if (_loggers[i]->logThisLoop()) {
                        _loggers[i]->log(_dataQ);
                        _loggers[i]->logThisLoop(false);
                    }
                }
                _dataQ->wrapEnd();
                _logThisLoop = false;
            }
        }

    private:
        JsonLogger **_loggers;
        DataQueue *_dataQ;
        uint16_t _numLoggers;
        bool _logThisLoop;
};

#endif