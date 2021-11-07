#ifndef _LOG_COMMAND_H
#define _LOG_COMMAND_H

#include "IntervalCommand.h"
#include "DataQueue.h"
#include "Sensor.h"
#include "Particle.h"

/**
 *  Templated class which represents a telemetry-logging command
 **/
template <class S, class R>
class LogCommand : public IntervalCommand {
    public:
        /**
         * Constructs a LogCommand with sensor, property name, getter function and interval
         * 
         * @param dataQ pointer to DataQueue in main
         * @param sensor pointer to sensor which we will call _getter on
         * @param propertyName the name of property which will be logged
         * @param func pointer to Sensor's member getter function
         * @param interval interval (in seconds) at which this command will be called to execute
         **/
        LogCommand(DataQueue *dataQ, S *sensor, String propertyName, R (S::*func)(), uint16_t interval)
        : IntervalCommand(interval) {
            _dataQ = dataQ;
            _sensor = sensor;
            _getter = func;
            _propertyName = propertyName;
        }

        ~LogCommand() { }

        /**
         * Constructs a LogCommand with sensor, property name, getter function and interval
         * 
         * @param dataQ passed in so getter data can be logged to main DataQueue
         **/
        void execute() {
            _dataQ->add(_propertyName, (*_sensor.*_getter)());
        }

    private:
        DataQueue *_dataQ;
        R (S::*_getter)();
        S *_sensor;
        String _propertyName;
};

#endif