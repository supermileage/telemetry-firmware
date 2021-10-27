#ifndef _SENSOR_CAN_LOG_COMMAND_H
#define _SENSOR_CAN_LOG_COMMAND_H

#include "Command.h"
#include "DataQueue.h"
#include "SensorGps.h"

class LogGpsCommand : public Command {
    public:
        LogGpsCommand(SensorGps *sensor, DataQueue *dataQ);

        void execute();
    private:
        SensorGps *_sensor;
        DataQueue *_dataQ;
};

#endif