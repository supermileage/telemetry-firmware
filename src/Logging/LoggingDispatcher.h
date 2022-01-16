#ifndef _LOGGING_DISPATCHER_H_
#define _LOGGING_DISPATCHER_H_

#include "settings.h"
#include "DataQueue.h"
#include "Handleable.h"
#include "IntervalCommandGroup.h"

/**
 * LoggingDispatcher owns and operates on a collection of commandGroups (commandGroup == collection of commands which share same execution interval)
 **/
class LoggingDispatcher : public Handleable {
    public:
        /**
         * Constructs a LoggingDispatcher with commandGroups, numCommandGroups, dataQueue and publishName
         **/
        LoggingDispatcher(IntervalCommandGroup **commandGroups, uint16_t numCommandGroups, DataQueue *dataQ, String publishName);

        /**
         * @brief Destroy the LoggingDispatcher object
         */
        ~LoggingDispatcher();

        /**
         *  Must be called from main loop!  Takes times since program start (in seconds) and checks whether
         *  execute needs to be called on any of its commandGroups
         **/
        void handle();

        void begin() { }

        /**
         * @brief turns on/off logging and publishing: will publish if turned off with data in buffer 
         */
        void setLoggingEnabled(bool value);

    private:
        DataQueue *_dataQ;
        uint16_t* _maxPublishSizes;
        IntervalCommandGroup **_commandGroups;
		uint16_t _numCommandGroups;
        bool _loggingEnabled = LOGGING_EN_AT_BOOT;
        bool _logThisLoop = FALSE;
        size_t _handleTime = 0;
        String _publishName;

        void _runLogging();

        void _publish();

        void _checkAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i);
};

#endif