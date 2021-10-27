#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include "Particle.h"
#include "Command.h"
#include "DataQueue.h"

class Dispatcher {
    public:
        Dispatcher(Command *commands, DataQueue *dataQ, uint8_t len, uint16_t interval);
        void run(uint16_t time);
    protected:
        void dispatch();
    private:
        Command *_commands;
        DataQueue *_dataQ;
        unsigned int _lastDispatch;
        unsigned int _numCommands;
        unsigned int _interval;
};

#endif