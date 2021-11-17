#include "DispatcherBuilder.h"

DispatcherBuilder::DispatcherBuilder(IntervalCommand *commands[], uint16_t numCommands, DataQueue *dataQ) {
    _dataQ = dataQ;
    _numIntervals = 0;
    _numCommands = numCommands;
    _commands = commands;
    _intervals = new uint16_t[MAX_NUM_INTERVALS] ();
    _numCommandsAddedOnIntervals = new uint16_t[MAX_NUM_INTERVALS] ();

    for (uint16_t i = 0; i < _numCommands; i++) {
        addInterval(commands[i]);
    }
}

DispatcherBuilder::~DispatcherBuilder() {
    delete[] _intervals;
    delete[] _numCommandsAddedOnIntervals;
}

Dispatcher* DispatcherBuilder::build() {
    // create array of *IntervalCommandGroup
    IntervalCommandGroup **loggers = new IntervalCommandGroup*[_numIntervals];

    // iterate over intervals: get all commands which share same interval and add them to a new logger
    // add loggers to pointer array of loggers
    for (int i = 0; i < _numIntervals; i++) {
        uint16_t interval = _intervals[i];
        uint16_t numCommandsOnInterval = _numCommandsAddedOnIntervals[i];
        Command **commandsOnInterval = new Command*[numCommandsOnInterval];
        
        uint16_t commandCount = 0;
        for (uint16_t i = 0; i < _numCommands; i++) {
            if (_commands[i]->getInterval() == interval)
                commandsOnInterval[commandCount++] = _commands[i];
        }
        loggers[i] = new IntervalCommandGroup(commandsOnInterval, numCommandsOnInterval, interval);
    }

    // create new dispatcher with set of loggers (_numIntervals == number of loggers)
    return new Dispatcher(loggers, _numIntervals, _dataQ);
}


void DispatcherBuilder::addInterval(IntervalCommand *command) {

    if (!containsInterval(command->getInterval())) {
        _intervals[_numIntervals++] = command->getInterval(); 
    }

    updateIntervals(command->getInterval());
}

bool DispatcherBuilder::containsInterval(uint16_t newInterval) {
    for (uint16_t i = 0; i < _numIntervals; i++) {
        if (newInterval == _intervals[i])
            return true;
    }
    return false;
}

void DispatcherBuilder::updateIntervals(uint16_t newInterval) {
    for (uint16_t i = 0; i < _numIntervals; i++) {
        if (newInterval == _intervals[i])
            ++_numCommandsAddedOnIntervals[i];
    } 
}