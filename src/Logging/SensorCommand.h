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
         * @param dataQ pointer to DataQueue in main
         * @param sensor pointer to sensor which we will call _getter on
         * @param propertyName the name of property which will be logged
         * @param func pointer to Sensor's member getter function
         * @param interval interval (in seconds) at which this command will be called to execute
         **/
        SensorCommand(DataQueue *dataQ, S *sensor, String propertyName, R (S::*func)(), uint16_t interval)
        : IntervalCommand(interval) {
            _dataQ = dataQ;
            _sensor = sensor;
            _getter = func;
            _propertyName = propertyName;
        }

        ~SensorCommand() { }

        /**
         * Logs data from this command's logging fn
         * 
         * @param dataQ passed in so getter data can be logged to main DataQueue
         **/
        void* execute(CommandArgs args) {
            _dataQ->add(_propertyName, (*_sensor.*_getter)());
            return (void*)nullptr;
        }

    private:
        DataQueue *_dataQ;
        R (S::*_getter)();
        S *_sensor;
        String _propertyName;
};

#endif