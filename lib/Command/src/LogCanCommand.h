#ifndef _LOG_CAN_COMMAND_H
#define _LOG_CAN_COMMAND_H

#include "Command.h"
#include "SensorCan.h"
#include "DataQueue.h"

class LogCanCommand : public Command {
    public:
        LogCanCommand(SensorCan *sensor, DataQueue *dataQ);
        
        void execute();
    private:
        SensorCan *_sensor;
        DataQueue *_dataQ;
};

#endif