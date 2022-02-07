#include "LoggingDispatcher.h"

#define DATAOBJECT_AND_TIMESTAMP_SIZE 23

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
            _commandGroups[i]->setExecuteThisLoop(true);
        }
    }

    // check max publish sizes, publish if DataQueue buffer is close to full, update max publish sizes for each logger
    if (_logThisLoop) {
        bool createNewDataObject = true;
        JsonObject dataObject;
        for (uint16_t i = 0; i < _numCommandGroups; i++) {
            // NOTE: creating new JsonObject -> {"t":1642311306,"d":{}} = 23 Bytes
            unsigned additionalBytes = createNewDataObject ? DATAOBJECT_AND_TIMESTAMP_SIZE : 0;
            if (_commandGroups[i]->getExecuteThisLoop() && _dataQ->getDataSize() + _maxPublishSizes[i] + additionalBytes >= _dataQ->getBufferSize()) {
                _publish();
                createNewDataObject = true;
            }

            if (_commandGroups[i]->getExecuteThisLoop()) {
                if (createNewDataObject) {
                    dataObject = _dataQ->createDataObject();
                    createNewDataObject = false;
                }
                uint16_t dataSizeBeforePublish = _dataQ->getDataSize();

                _commandGroups[i]->executeCommands((CommandArgs)&dataObject);
                _commandGroups[i]->setExecuteThisLoop(false);

                uint16_t dataSizeAfterPublish = _dataQ->getDataSize();

                _checkAndUpdateMaxPublishSizes(dataSizeAfterPublish - dataSizeBeforePublish, i);
            }
        }
        _logThisLoop = false;

        // If there is a problem with json, call publish to reset dataQueue
        if (!_dataQ->verifyJsonStatus()) {
            _publish();
        }
    }
}

void LoggingDispatcher::_publish() {
    _dataQ->publish(_publishName, PRIVATE, WITH_ACK);
}

void LoggingDispatcher::_checkAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i) {
    if (currentPublishSize > _maxPublishSizes[i])
        _maxPublishSizes[i] = currentPublishSize;
}