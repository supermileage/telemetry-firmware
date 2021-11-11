#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "Particle.h"
#include "DataQueue.h"
#include "IntervalCommandGroup.h"

/**
 * Dispatcher owns and operates on a collection of loggers
 **/
class Dispatcher {
    public:
        /**
         * Constructs a Dispatcher with loggers, numLoggers and DataQueue
         **/
        Dispatcher(IntervalCommandGroup **loggers, uint16_t numLoggers, DataQueue *dataQ);

        /**
         * @brief Destroy the Dispatcher object
         */
        ~Dispatcher();

        /**
         *  Must be called from main loop!  Takes times since program start (in seconds) and checks whether
         *  log needs to be called on any of its loggers.
         * 
         * @param time the time in seconds since the start of the program
         **/
        void run();

    private:
        IntervalCommandGroup **_loggers;
        DataQueue *_dataQ;
        uint16_t _numLoggers;
        bool _logThisLoop;
};

#endif