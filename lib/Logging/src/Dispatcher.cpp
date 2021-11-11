        #include "Dispatcher.h"
		
		Dispatcher::Dispatcher(IntervalCommandGroup **loggers, uint16_t numLoggers, DataQueue *dataQ) {
            _loggers = loggers;
            _numLoggers = numLoggers;
            _dataQ = dataQ;
            _logThisLoop = false;
        }

        Dispatcher::~Dispatcher() {
            for (uint16_t i = 0; i < _numLoggers; i++) {
                delete _loggers[i];
            }
            delete[] _loggers;
        }

        void Dispatcher::run() {
            unsigned long time = millis() / 1000;
            // check if it's time to log any data from any of the loggers
            for (uint16_t i = 0; i < _numLoggers; i++) {
                if ((_loggers[i]->getLastExecution() + _loggers[i]->getInterval()) <= time) {
                    _loggers[i]->setLastExecution(time);
                    _logThisLoop = true;
                    _loggers[i]->executeThisLoop(true);
                }   
            }

            // call log on loggers and reset logThisLoop bool
            if (_logThisLoop) {
                _dataQ->wrapStart();
                for (uint16_t i = 0; i < _numLoggers; i++) {
                    if (_loggers[i]->executeThisLoop()) {
                        _loggers[i]->executeCommands();
                        _loggers[i]->executeThisLoop(false);
                    }
                }
                _dataQ->wrapEnd();
                _logThisLoop = false;   
            }
        }