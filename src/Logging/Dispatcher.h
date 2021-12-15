#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "settings.h"
#include "Handleable.h"
#include "DataQueue.h"
#include "IntervalCommandGroup.h"

/**
 * Dispatcher owns and operates on a collection of loggers
 **/
class Dispatcher : public Handleable {
    public:
        /**
         * Constructs a Dispatcher with loggers, numLoggers, dataQueue and publishName
         **/
        Dispatcher(IntervalCommandGroup **loggers, uint16_t numLoggers, DataQueue *dataQ, String publishName);

        /**
         * @brief Destroy the Dispatcher object
         */
        ~Dispatcher();

        void begin();

        void setEnableLogging(bool value);

        /**
         *  Must be called from main loop!  Takes times since program start (in seconds) and checks whether
         *  execute needs to be called on any of its loggers
         **/
        void handle();

    private:
        bool _loggingEnabled = LOGGING_EN_AT_BOOT;

        IntervalCommandGroup **_loggers;
        DataQueue *_dataQ;
        uint16_t* _maxPublishSizes;
        uint16_t _numLoggers;
        bool _logThisLoop;
        String _publishName;

        void _runLogging();

        void _checkAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i);
};

#endif