#include "DataQueue.h"

#ifndef _COMMAND_H
#define _COMMAND_H

/**
 *  Abstract Command class
 **/
class Command {
    public:
        /**
         * Default constructor
         **/
        Command() { }

        virtual void execute(DataQueue *dataQ) = 0;

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