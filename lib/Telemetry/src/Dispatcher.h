#include "Particle.h"

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

class Dispatcher {
    public:
        Dispatcher(JsonLogger **loggers, uint16_t numLoggers) {
            _loggers = loggers;
            _numLoggers = numLoggers;
            _logThisLoop = false;
        }

        void run(unsigned long time) {

            // check if any of the loggers have reached the start of new interval
            for (uint16_t i = 0; i < _numLoggers; i++) {
                if ((_loggers[i]->getLastLog() + _loggers[i]->getInterval()) >= time) {
                    _logThisLoop = true;
                    _loggers[i]->logThisLoop(true);
                }   
            }

            // call log on whichever should be logged, then reset logThisLoop bool on this and loggers
            if (_logThisLoop) {
                dataQ->wrapStart();
                for (uint16_t i = 0; i < _numLoggers; i++) {
                    if (_loggers[i]->logThisLoop()) {
                        _loggers[i]->logThisLoop(false);
                        _loggers[i]->log();
                        _loggers[i]->setLastLog(time);
                    }
                }

                dataQ->wrapEnd();
                _logThisLoop = false;
            }
        }

    private:
        JsonLogger **_loggers;
        bool _logThisLoop;
        uint16_t _numLoggers;
};

#endif