#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "DataQueue.h"
#include "IntervalCommandGroup.h"

/**
 * Dispatcher owns and operates on a collection of loggers
 **/
class Dispatcher {
    public:
        /**
         * Constructs a Dispatcher with loggers, numLoggers, dataQueue and publishName
         **/
        Dispatcher(IntervalCommandGroup **loggers, uint16_t numLoggers, DataQueue *dataQ, String publishName);

        /**
         * @brief Destroy the Dispatcher object
         */
        ~Dispatcher();

        /**
         *  Must be called from main loop!  Takes times since program start (in seconds) and checks whether
         *  execute needs to be called on any of its loggers
         **/
        void loop();

    private:
        void CheckAndUpdateMaxPublishSizes(uint16_t currentPublishSize, uint16_t i);

        IntervalCommandGroup **_loggers;
        DataQueue *_dataQ;
        uint16_t* _maxPublishSizes;
        uint16_t _numLoggers;
        bool _logThisLoop;
        String _publishName;
};

#endif