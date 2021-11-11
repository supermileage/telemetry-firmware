#include "DataQueue.h"

#ifndef _COMMAND_H
#define _COMMAND_H

/**
 *  Abstract Command class
 **/
class Command {
    public:
        Command() { }

        virtual ~Command() { }

        /**
         * Virtual function executes the instruction owned by this command
         **/
        virtual void execute() = 0;
};

#endif