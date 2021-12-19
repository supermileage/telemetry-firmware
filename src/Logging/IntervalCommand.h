#ifndef _INTERVAL_COMMAND_H_
#define _INTERVAL_COMMAND_H_

#include "Command.h"

class IntervalCommand : public Command {
    public:
        /**
         * Default constructor
         **/
        IntervalCommand() { }

        /**
         * Constructs IntervalCommand with interval 
         **/
        IntervalCommand(uint16_t interval) {
            _interval = interval;
        }

        virtual ~IntervalCommand() { }

        /**
         * Virtual function executes the instruction owned by this command
         **/
        virtual void* execute(CommandArgs args) = 0;

        /**
         *  Returns interval assigned to this command (_interval is only assigned in constructor of subclasses)
         **/
        uint16_t getInterval() {
            return _interval;
        }
    protected:
        uint16_t _interval;
};

#endif