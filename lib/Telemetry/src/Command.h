#include "CommandBase.h"
#include "DataQueue.h"
#include "Sensor.h"
#include "Particle.h"

#ifndef _COMMAND_H
#define _COMMAND_H

template <class S, class R>
class Command : public CommandBase {
    public:
        Command(S *sensor, String propertyName, R (S::*func)(), uint16_t interval) {
            _sensor = sensor;
            _someGetter = func;
            _propertyName = propertyName;
            _interval = interval;
        }

        void execute(DataQueue *dataQ) {
            dataQ->add(_propertyName, (*_sensor.*_someGetter)());
        }

    private:
        R (S::*_someGetter)();
        S *_sensor;
        String _propertyName;
};

#endif