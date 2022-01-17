#include "LoggingDispatcher.h"

#define WRAPPER_START 25
#define WRAPPER_END 4

LoggingDispatcher::LoggingDispatcher(IntervalCommandGroup** commandGroups, uint16_t numCommandGroups, DataQueue* dataQ, String publishName) {
    _commandGroups = commandGroups;
    _numCommandGroups = numCommandGroups;
    _maxPublishSizes = new uint16_t[numCommandGroups]();
    _dataQ = dataQ;
    _logThisLoop = false;
    _publishName = publishName;
}

LoggingDispatcher::~LoggingDispatcher() {
    for (uint16_t i = 0; i < _numCommandGroups; i++) {
        delete _commandGroups[i];
    }
    delete[] _commandGroups;
    delete[] _maxPublishSizes;
}

void LoggingDispatcher::setLoggingEnabled(bool value) {
    bool newState = _loggingEnabled != value;
    _loggingEnabled = value;

    if (!value && newState) {
        // flush data in data queue
        _publish();
    }
}

void LoggingDispatcher::handle() {
    if(_loggingEnabled) {
        _runLogging();
    }
}

void LoggingDispatcher::_runLogging() {
    unsigned long time = millis() / 1000;
    // check if it's time to log any data from any of the commandGroups
    for (uint16_t i = 0; i < _numCommandGroups; i++) {
        if ((_commandGroups[i]->getLastExecution() + _commandGroups[i]->getInterval()) <= time) {
            _commandGroups[i]->setLastExecution(time);
            _logThisLoop = true;
            _commandGroups[i]->executeThisLoop(true);
        }
    }

    // check max publish sizes, publish if DataQueue buffer is close to full, update max publish sizes for each logger
    if (_logThisLoop) {
        bool dataWrapperIsOpen = false;
        for (uint16_t i = 0; i < _numCommandGroups; i++) {
            // NOTE: max wrapper opening size = 21 bytes; wrapper closing size = 2 bytes; final closing brackets = 2 bytes
            unsigned additionalBytes = dataWrapperIsOpen ? WRAPPER_END : WRAPPER_START;
            if (_dataQ->getDataSize() + _maxPublishSizes[i] + additionalBytes > _dataQ->getBufferSize() && _commandGroups[i]->executeThisLoop()) {
                if (dataWrapperIsOpen) {
                    _dataQ->wrapEnd();
                    dataWrapperIsOpen = false;
                }
                _publish();
            }

            if (_commandGroups[i]->executeThisLoop()) {
                if (!dataWrapperIsOpen) { 
                    _dataQ->wrapStart();
                    dataWrapperIsOpen = true;
                }
                uint16_t dataSizeBeforePublish = _dataQ->getDataSize();

                _commandGroups[i]->executeCommands((CommandArgs)_dataQ);
                _commandGroups[i]->executeThisLoop(false);

                uint16_t dataSizeAfterPublish = _dataQ->getDataSize();
                _checkAndUpdateMaxPublishSizes(dataSizeAfterPublish - dataSizeBeforePublish, i);
            }
        }
        if (dataWrapperIsOpen)
            _dataQ->wrapEnd();
            
        _logThisLoop = false;
    }
}

void LoggingDispatcher::_publish() {
    _dataQ->publish(_publishName, PRIVATE, WITH_ACK);
}

void LoggingDispatcher::_checkAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i) {
    if (currentPublishSize > _maxPublishSizes[i])
        _maxPublishSizes[i] = currentPublishSize;
}