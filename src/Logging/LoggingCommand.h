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
        LoggingCommand(C* object, String propertyName, R (C::*getter)(), uint16_t interval, bool (C::*validator)(R))
        : IntervalCommand(interval) {
            _object = object;
            _getter = getter;
            _propertyName = propertyName;
            _validator = validator;
            IntervalCommandHandler::instance().add(this);
        }
        
        ~LoggingCommand() { }

        /**
         * @brief Logs data from this command's getter method to JsonObject
         * 
         * @param args pointer to JsonObject
         */
        void execute(CommandArgs args) override {
            R val = (*_object.*_getter)();
            
            if ((*_object.*_validator)(val)) {
                (*(JsonObject*)args)[_propertyName] = val;
            }
        }

    private:
        R (C::*_getter)();
        bool (C::*_validator)(R);
        C *_object;
        String _propertyName;
};

#endif