#ifndef _COMMAND_H
#define _COMMAND_H

#include "DataQueue.h"

typedef void* CommandArgs;

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
        virtual void execute(CommandArgs args) = 0;
};

#endif