#include "Command.h"
#include "DataQueue.h"

#ifndef _INTERVAL_LOGGER_H_
#define _INTERVAL_LOGGER_H_

/**
 * IntervalLogger owns a collection of logging commands set to log on the same interval
**/
class IntervalLogger {
    public:
        /**
        * Default constructor
        **/
        IntervalLogger();

        ~IntervalLogger();

        /**
        *  Constructor
        * 
        * @param commands the set of commands which will be called to execute on this logger's timing interval
        * @param numCommands the number of commands which this logger owns
        * @param interval the interval (in seconds) on which this logger logs
        **/
        IntervalLogger(Command **commands, uint8_t numCommands, uint16_t interval);

        /**
        *  Calls execute on all commands owned by this logger
        **/
        void log(DataQueue *dataQ);

        /**
        *  Retuns the interval length (in seconds)
        **/
        uint16_t getInterval();

        /**
        *  Returns time at which this logger was last called to log
        **/
        unsigned long getLastLog();

        /**
        *  Sets time at which this this logger was last called to log
        **/
        void setLastLog(unsigned int time);

        /**
        *  Returns true if logger has been set to log
        **/
        bool logThisLoop();

        /**
        *  Sets bool for whether or not log should be called 
        **/
        void logThisLoop(bool logThisLoop);

    protected:
        unsigned long _lastLog;
        Command **_commands;
        uint16_t _interval;
        uint8_t _numCommands;
        bool _logThisLoop;
};

#endif