#include "DataQueue.h"
#include "LogCommand.h"
#include "Command.h"
#include "Dispatcher.h"
#include "Particle.h"
#include "IntervalLogger.h"

#define MAX_NUM_INTERVALS 5

#ifndef _DISPTACHER_FACTORY_H_
#define _DISPATCHER_FACTORY_H_

// This isn't really a factory but it has a similar purpose:
// Add all the commands you want to this and call build().  It
// produces a dispatcher with all logging functionality already set up
// You can delete this safely after build() has been called
class DispatcherFactory {
    public:
        /**
         * Constructs new DispatcherFactory with number of commands and data queue
         * 
         * @param numCommands The number of sensor functions you will add to the factory
         * Note: this must be accurate and has potential to cause a segfault if the value is too low
         * @param dataQ the Data Queue from main
         * */
        DispatcherFactory(uint16_t numCommands, DataQueue *dataQ) {
            _dataQ = dataQ;
            _commands = new Command*[numCommands];
            _intervals = new uint16_t[MAX_NUM_INTERVALS] ();
            _numCommandsOnIntervals = new uint16_t[MAX_NUM_INTERVALS] ();
            _numCommands = 0;
            _numIntervals = 0;
        }
        
        /**
         * Destructor which frees unused heap data (can safely be called after build)
        **/
        ~DispatcherFactory() {
            delete _intervals;
            delete _numCommandsOnIntervals;
        }

        /**
         * Adds a Sensor-property getter function, the property name, and interval at which to call it
         * 
         * @param S the address of the Sensor whose getter you want to be called
         * @param propertyName the publish-name of the property which the getter returns
         * @param func function pointer to Sensor member function with return type of R
         * @param interval the interval (in seconds) at which you want to log data from Sensor's getter
         **/
        template <class S, class R>
        void add(S *sensor, String propertyName, R (S::*func)(), uint16_t interval) {
            _commands[_numCommands++] = new LogCommand<S, R>(sensor, propertyName, func, interval);

            if (!containsInterval(interval)) {
                _intervals[_numIntervals++] = interval; 
            }

            updateIntervals(interval);
        }

        /**
         * Builds and returns a dispatcher with all of the logging information you've added
         **/
        Dispatcher* build() {
            // create array of *IntervalLogger
            IntervalLogger **loggers = new IntervalLogger*[_numIntervals];

            // iterate over intervals: get all commands which share same interval and add them to a new logger
            // add loggers to pointer array of loggers
            for (int i = 0; i < _numIntervals; i++) {
                uint16_t interval = _intervals[i];
                uint16_t numCommandsOnInterval = _numCommandsOnIntervals[i];
                Command **commandsOnInterval = new Command*[numCommandsOnInterval];
                
                uint16_t commandCount = 0;
                for (uint16_t i = 0; i < _numCommands; i++) {
                    if (_commands[i]->getInterval() == interval)
                        commandsOnInterval[commandCount++] = _commands[i];
                }
                loggers[i] = new IntervalLogger(commandsOnInterval, numCommandsOnInterval, interval);
            }

            // create new dispatcher with set of loggers (_numIntervals == number of loggers)
            return new Dispatcher(loggers, _numIntervals, _dataQ);
        }

    private:
        // return true if array of intervals already contains this interval
        bool containsInterval(uint16_t newInterval) {
            for (uint16_t i = 0; i < _numIntervals; i++) {
                if (newInterval == _intervals[i])
                    return true;
            }
            return false;
        }

        // keeps track of how many commands are on the same interval
        void updateIntervals(uint16_t newInterval) {
            for (uint16_t i = 0; i < _numIntervals; i++) {
                if (newInterval == _intervals[i])
                    ++_numCommandsOnIntervals[i];
            } 
        }

        Command **_commands;
        DataQueue *_dataQ;
        uint16_t _numCommands;
        uint16_t *_intervals;
        uint16_t *_numCommandsOnIntervals;
        uint16_t _numIntervals;
};

#endif