#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "DataQueue.h"
#include "IntervalCommandGroup.h"

/**
 * LoggingDispatcher owns and operates on a collection of loggers
 **/
class LoggingDispatcher {
    public:
        /**
         * Constructs a LoggingDispatcher with loggers, numLoggers, dataQueue and publishName
         **/
        LoggingDispatcher(IntervalCommandGroup **loggers, uint16_t numLoggers, DataQueue *dataQ, String publishName);

        /**
         * @brief Destroy the LoggingDispatcher object
         */
        ~LoggingDispatcher();

        /**
         *  Must be called from main loop!  Takes times since program start (in seconds) and checks whether
         *  execute needs to be called on any of its loggers
         **/
        void loop();

    private:
        IntervalCommandGroup **_loggers;
        DataQueue *_dataQ;
        uint16_t* _maxPublishSizes;
        uint16_t _numLoggers;
        bool _logThisLoop;
        String _publishName;

        void CheckAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i);
};

#endif