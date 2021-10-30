#ifndef _CAN_LOGGER_H
#define _CAN_LOGGER_H

#include "Command.h"
#include "SensorCan.h"
#include "DataQueue.h"

class CanLogger : public Command {
    public:
        CanLogger(SensorCan *sensor);

        void execute(DataQueue *dataQ) = 0;
    protected:
        SensorCan *_sensor;
};

#endif