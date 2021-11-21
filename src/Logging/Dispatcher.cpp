#include "Dispatcher.h"

Dispatcher::Dispatcher(IntervalCommandGroup **loggers, uint16_t numLoggers, DataQueue *dataQ) {
    _loggers = loggers;
    _numLoggers = numLoggers;
    _maxPublishSizes = new uint16_t[numLoggers]();
    _dataQ = dataQ;
    _logThisLoop = false;
}

Dispatcher::~Dispatcher() {
    for (uint16_t i = 0; i < _numLoggers; i++) {
        delete _loggers[i];
    }
    delete[] _loggers;
}

void Dispatcher::loop() {
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
        bool firstPass = true;
        for (uint16_t i = 0; i < _numLoggers; i++) {
            if (_dataQ->getDataSize() + _maxPublishSizes[i] + 2 > _dataQ->getBufferSize() && _loggers[i]->executeThisLoop())
                _dataQ->publish("BQIngestion", PRIVATE, WITH_ACK);
            
            if (firstPass) { 
                _dataQ->wrapStart();
                firstPass = false;
            }

            if (_loggers[i]->executeThisLoop()) {
                uint16_t dataSizeBeforePublish = _dataQ->getDataSize();

                _loggers[i]->executeCommands();
                _loggers[i]->executeThisLoop(false);

                uint16_t dataSizeAfterPublish = _dataQ->getDataSize();
                CheckAndUpdateMaxPublishSizes(dataSizeAfterPublish - dataSizeBeforePublish, i);
            }
        }
        _dataQ->wrapEnd();
        _logThisLoop = false;
    }
}

void Dispatcher::CheckAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i) {
    if (currentPublishSize > _maxPublishSizes[i])
        _maxPublishSizes[i] = currentPublishSize;
}