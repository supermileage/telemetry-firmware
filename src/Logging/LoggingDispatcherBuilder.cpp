#include "LoggingDispatcherBuilder.h"

LoggingDispatcherBuilder::LoggingDispatcherBuilder(IntervalCommand *commands[], DataQueue *dataQ, String publishName) {
    _dataQ = dataQ;
    _commands = commands;
    _publishName = publishName;

    // add command intervals to map and increment interval counts
    for (uint16_t i = 0; commands[i]; i++) {
        ++_intervalMap[commands[i]->getInterval()];
    }
}

LoggingDispatcherBuilder::~LoggingDispatcherBuilder() { }

LoggingDispatcher* LoggingDispatcherBuilder::build() {
    uint16_t numIntervals = _intervalMap.size();
    IntervalCommandGroup **commandGroups = new IntervalCommandGroup*[numIntervals];
    uint16_t i = 0;

    // iterate over intervalMap, get all commands which share same interval and add them to a new commandGroup,
    // then pass commandGroups to LoggingDispatcher constructor
    for (auto const& pair : _intervalMap) {
        uint16_t interval = pair.first;
        uint16_t numCommandsOnInterval = pair.second;
        Command **commandsOnInterval = new Command*[numCommandsOnInterval];

        uint16_t commandCount = 0;
        for (uint16_t i = 0; _commands[i]; i++) {
            if (_commands[i]->getInterval() == interval)
                commandsOnInterval[commandCount++] = _commands[i];
        }
        commandGroups[i++] = new IntervalCommandGroup(commandsOnInterval, numCommandsOnInterval, interval);
    }

    return new LoggingDispatcher(commandGroups, numIntervals, _dataQ, _publishName);
}