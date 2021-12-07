#include "Dispatcher.h"

Dispatcher::Dispatcher(IntervalCommandGroup **loggers, uint16_t numLoggers, DataQueue *dataQ, String publishName) {
    _loggers = loggers;
    _numLoggers = numLoggers;
    _maxPublishSizes = new uint16_t[numLoggers]();
    _dataQ = dataQ;
    _logThisLoop = false;
    _publishName = publishName;
}

Dispatcher::~Dispatcher() {
    for (uint16_t i = 0; i < _numLoggers; i++) {
        delete _loggers[i];
    }
    delete[] _loggers;
    delete[] _maxPublishSizes;
}

void Dispatcher::begin() {}

void Dispatcher::enableLogging() {
    _loggingEnabled = TRUE;
}

void Dispatcher::disableLogging() {
    _loggingEnabled = FALSE;
    // TODO: Code for flushing buffer
}

void Dispatcher::handle() {
    if(_loggingEnabled) {
        _runLogging();
    }
}

void Dispatcher::_runLogging() {
    unsigned long time = millis() / 1000;
    // check if it's time to log any data from any of the loggers
    for (uint16_t i = 0; i < _numLoggers; i++) {
        if ((_loggers[i]->getLastExecution() + _loggers[i]->getInterval()) <= time) {
            _loggers[i]->setLastExecution(time);
            _logThisLoop = true;
            _loggers[i]->executeThisLoop(true);
        }   
    }

    // check max publish sizes, publish if DataQueue buffer is close to full, update max publish sizes for each logger
    if (_logThisLoop) {
        bool dataWrapperIsOpen = false;
        for (uint16_t i = 0; i < _numLoggers; i++) {
            // NOTE: max wrapper opening size = 21 bytes; wrapper closing size = 2 bytes; final closing brackets = 2 bytes
            unsigned additionalBytes = dataWrapperIsOpen ? 4 : 25;
            if (_dataQ->getDataSize() + _maxPublishSizes[i] + additionalBytes > _dataQ->getBufferSize() && _loggers[i]->executeThisLoop()) {
                if (dataWrapperIsOpen) {
                    _dataQ->wrapEnd();
                    dataWrapperIsOpen = false;
                }

                _dataQ->publish(_publishName, PRIVATE, WITH_ACK);
            }

            if (_loggers[i]->executeThisLoop()) {
                if (!dataWrapperIsOpen) { 
                    _dataQ->wrapStart();
                    dataWrapperIsOpen = true;
                }
                uint16_t dataSizeBeforePublish = _dataQ->getDataSize();

                _loggers[i]->executeCommands();
                _loggers[i]->executeThisLoop(false);

                uint16_t dataSizeAfterPublish = _dataQ->getDataSize();
                CheckAndUpdateMaxPublishSizes(dataSizeAfterPublish - dataSizeBeforePublish, i);
            }
        }
        if (dataWrapperIsOpen)
            _dataQ->wrapEnd();

        _logThisLoop = false;
    }
}


void Dispatcher::CheckAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i) {
    if (currentPublishSize > _maxPublishSizes[i])
        _maxPublishSizes[i] = currentPublishSize;
}