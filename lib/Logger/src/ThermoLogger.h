#ifndef _THERMO_LOGGER_H_
#define _THERMO_LOGGER_H_

#include "SensorThermo.h"
#include "DataQueue.h"
#include "Command.h"

class ThermoLogger : public Command {
    public:
        ThermoLogger(SensorThermo *sensor);
        void execute(DataQueue *dataQ) = 0;
    protected:
        SensorThermo *_sensor;
};

#endif