#ifndef _object_COMMAND_H
#define _object_COMMAND_H

#include "IntervalCommand.h"
#include "DataQueue.h"

/**
 *  Templated Command class which represents a telemetry-logging command
 *  class C is the type of object whose member function will be called on execute
 *  class R is the return type of that member function
 **/
template <class C, class R>
class LoggingCommand : public IntervalCommand {
    public:
        /**
         * Constructs a LoggingCommand with object, property name, getter function and interval
         * 
         * @param object pointer to object of class C which we will call _getter on
         * @param propertyName the name of property which will be logged
         * @param func pointer to object's member getter function
         * @param interval interval (in seconds) at which this command will be called to execute
         **/
        LoggingCommand(C *object, String propertyName, R (C::*func)(), uint16_t interval)
        : IntervalCommand(interval) {
            _object = object;
            _getter = func;
            _propertyName = propertyName;
        }
        
        ~LoggingCommand() { }

        /**
         * Logs data from this command's getter method
         * 
         **/
        void execute(CommandArgs args) {
            ((DataQueue*)args)->add(_propertyName, (*_object.*_getter)());
        }

    private:
        R (C::*_getter)();
        C *_object;
        String _propertyName;
};

#endif