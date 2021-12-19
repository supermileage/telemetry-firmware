#ifndef _COMMAND_H
#define _COMMAND_H

#include "DataQueue.h"

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
        virtual void* execute(void* arg) = 0;
};

#endif