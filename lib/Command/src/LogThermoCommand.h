#ifndef _LOG_THERMO_COMMAND_H
#define _LOG_THERMO_COMMAND_H

#include "SensorThermo.h"
#include "DataQueue.h"
#include "Command.h"

class LogThermoCommand : public Command {
    public:
        LogThermoCommand(SensorThermo *sensor, DataQueue *dataQ);
        void execute();
    private:
        SensorThermo *_sensor;
        DataQueue *_dataQ;
};

#endif