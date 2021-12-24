#ifndef _SENSOR_COMMAND_H
#define _SENSOR_COMMAND_H

#include "IntervalCommand.h"
#include "DataQueue.h"
#include "Sensor.h"

/**
 *  Templated class which represents a telemetry-logging command
 **/
template <class S, class R>
class SensorCommand : public IntervalCommand {
    public:
        /**
         * Constructs a SensorCommand with sensor, property name, getter function and interval
         * 
         * @param sensor pointer to sensor which we will call _getter on
         * @param propertyName the name of property which will be logged
         * @param func pointer to Sensor's member getter function
         * @param interval interval (in seconds) at which this command will be called to execute
         **/
        SensorCommand(S *sensor, String propertyName, R (S::*func)(), uint16_t interval)
        : IntervalCommand(interval) {
            _sensor = sensor;
            _getter = func;
            _propertyName = propertyName;
        }

        ~SensorCommand() { }

        /**
         * Logs data from this command's getter method
         * 
         **/
        void execute(CommandArgs args) {
            ((DataQueue*)args)->add(_propertyName, (*_sensor.*_getter)());
        }

    private:
        R (S::*_getter)();
        S *_sensor;
        String _propertyName;
};

#endif