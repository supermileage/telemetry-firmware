#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "Command.h"
#include "DataQueue.h"

class Dispatcher {
    public:
        Dispatcher(Command *loggers, DataQueue *dataQ, uint8_t len, uint16_t interval);
        void run(uint32_t time);
    protected:
        void dispatch();
    private:
        Command *_loggers;
        DataQueue *_dataQ;
        uint16_t _interval;
        uint8_t _numCommands;
        uint32_t _lastDispatch; 
};

#endif