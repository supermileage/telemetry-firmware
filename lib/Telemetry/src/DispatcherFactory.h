#include "DataQueue.h"
#include "Command.h"
#include "CommandBase.h"
#include "Dispatcher.h"
#include "Particle.h"
#include "JsonLogger.h"

#define MAX_NUM_INTERVALS 5

#ifndef _DISPTACHER_FACTORY_H_
#define _DISPATCHER_FACTORY_H_

class DispatcherFactory {
    public:
        DispatcherFactory() { }

        DispatcherFactory(uint16_t initArraySize, DataQueue *dataQ) {
            _dataQ = dataQ;
            _commandArraySize = initArraySize;
            _commands = new CommandBase*[initArraySize];
            _intervals = new uint16_t[MAX_NUM_INTERVALS] ();
            _numCommandsOnIntervals = new uint16_t[MAX_NUM_INTERVALS] ();
            _numCommands = 0;
            _numIntervals = 0;
        }
        
        // would be very good to review what needs to be deleted with someone else
        ~DispatcherFactory() { }

        template <class S, class R>
        void add(S *sensor, String propertyName, R (S::*func)(), uint16_t interval) {
            _commands[_numCommands++] = new Command<S, R>(sensor, propertyName, func, interval);

            if (!containsInterval(interval)) {
                _intervals[_numIntervals++] = interval; 
            }

            addToIntervalsCount(interval);
        }

        Dispatcher* build() {
            JsonLogger **loggers = new JsonLogger*[_numIntervals];

            for (int i = 0; i < _numIntervals; i++) {
                uint16_t interval = _intervals[i];
                uint16_t numCommandsOnInterval = _numCommandsOnIntervals[i];
                CommandBase **commandsOnInterval = new CommandBase*[numCommandsOnInterval];
                
                uint16_t commandCount = 0;
                for (uint16_t i = 0; i < _numCommands; i++) {
                    if (_commands[i]->getInterval() == interval)
                        commandsOnInterval[commandCount++] = _commands[i];
                }

                loggers[i] = new JsonLogger(commandsOnInterval, numCommandsOnInterval, interval);
            }

            return new Dispatcher(loggers, _numIntervals, _dataQ);
        }

    private:
        bool containsInterval(uint16_t newInterval) {
            for (uint16_t i = 0; i < _numIntervals; i++) {
                if (newInterval == _intervals[i])
                    return true;
            }
            return false;
        }

        void addToIntervalsCount(uint16_t newInterval) {
            for (uint16_t i = 0; i < _numIntervals; i++) {
                if (newInterval == _intervals[i])
                    ++_numCommandsOnIntervals[i];
            } 
        }

        CommandBase **_commands;
        DataQueue *_dataQ;
        uint16_t _numCommands;
        uint16_t *_intervals;
        uint16_t *_numCommandsOnIntervals;
        uint16_t _numIntervals;
        uint16_t _commandArraySize;
};

#endif