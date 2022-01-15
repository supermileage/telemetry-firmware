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
<<<<<<< HEAD:src/System/Command.h
        virtual void* execute(void* arg) = 0;
=======
        virtual void execute(CommandArgs args) = 0;
>>>>>>> develop:src/Logging/Command.h
};

#endif