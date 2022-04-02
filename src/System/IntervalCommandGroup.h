#ifndef _INTERVAL_COMMAND_GROUP_H_
#define _INTERVAL_COMMAND_GROUP_H_

#include "Command.h"

/**
 * IntervalCommandGroup owns a collection of commands on the same execution interval
**/
class IntervalCommandGroup {
    public:
        /**
        * Default constructor
        **/
        IntervalCommandGroup();

        ~IntervalCommandGroup();

        /**
        *  Constructor
        * 
        * @param commands the set of commands which will be called to execute on this groups timing interval
        * @param numCommands the number of commands in this group
        * @param interval the interval (in seconds) on which this group executes
        **/
        IntervalCommandGroup(Command **commands, uint8_t numCommands, uint16_t interval);

        /**
        *  Calls execute on all commands owned by this group
        **/
        void executeCommands(CommandArgs args);

        /**
        *  Retuns the interval length (in seconds)
        **/
        uint16_t getInterval();

        /**
        *  Returns time at which this group was last called to execute
        **/
        unsigned long getLastExecution();

        /**
        *  Sets time at which this this group was last called to execute
        **/
        void setLastExecution(unsigned int time);

        /**
        *  Returns true if group has been set to execute
        **/
        bool getExecuteThisLoop();

        /**
        *  Sets bool for whether or not executeCommands should be called 
        **/
        void setExecuteThisLoop(bool executeThisLoop);

    protected:
        unsigned long _lastExecution;
        Command **_commands;
        uint16_t _interval;
        uint8_t _numCommands;
        bool _executeThisLoop;
};

#endif