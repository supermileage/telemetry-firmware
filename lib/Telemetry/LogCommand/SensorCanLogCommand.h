#ifndef _SENSOR_CAN_LOG_COMMAND_H
#define _SENSOR_CAN_LOG_COMMAND_H

#include "DataQueue.h"
#include "SensorCan.h"

class SensorCanLogCommand : public virtual LogCommand {
    public:
        SensorCanLogCommand(SensorCan &sensor);
    private:
        SensorCan *_sensor;
};

#endif