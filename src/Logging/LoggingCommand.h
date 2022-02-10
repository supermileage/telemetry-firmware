#ifndef _object_COMMAND_H
#define _object_COMMAND_H

#include "ArduinoJson.h"
#include "IntervalCommand.h"

/**
 *  Templated Command class which represents a telemetry-logging command
 *  class C is the type of object whose getter method will be called on execute
 *  class R is the return type of getter
 **/
template <class C, class R>
class LoggingCommand : public IntervalCommand {
    public:
        /**
         * Constructs a LoggingCommand with object, property name, getter method pointer and interval.
         * Adds this to static collection of interval commands
         * 
         * @param object pointer to object of class C which we will call _getter on
         * @param propertyName the name of property which will be logged
         * @param getter pointer to object's getter method
         * @param interval interval (in seconds) at which this command will be called to execute
         **/
        LoggingCommand(C* object, String propertyName, R (C::*getter)(), uint16_t interval)
        : IntervalCommand(interval) {
            _object = object;
            _getter = getter;
            _propertyName = propertyName;
            IntervalCommandHandler::instance().add(this);
        }
        
        ~LoggingCommand() { }

        /**
         * @brief Logs data from this command's getter method to JsonObject
         * 
         * @param args pointer to JsonObject
         */
        void execute(CommandArgs args) override {
            (*(JsonObject*)args)[_propertyName] = (*_object.*_getter)();
        }

    private:
        R (C::*_getter)();
        C *_object;
        String _propertyName;
};

#endif