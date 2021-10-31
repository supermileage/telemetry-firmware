#include "DataQueue.h"

#ifndef _COMMAND_BASE_H
#define _COMMAND_BASE_H

class CommandBase {
    public:
        virtual void execute(DataQueue *dataQ) = 0;

        uint16_t getInterval() {
            return _interval;
        }

    protected:
         uint16_t _interval;
};

#endif