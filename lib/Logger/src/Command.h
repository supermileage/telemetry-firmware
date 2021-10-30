#ifndef _COMMAND_H
#define _COMMAND_H

#include "DataQueue.h"

class Command {
    public:
        virtual void execute(DataQueue *dataQ) = 0;
};

#endif