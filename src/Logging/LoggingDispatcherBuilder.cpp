#include "LoggingDispatcherBuilder.h"

LoggingDispatcherBuilder::LoggingDispatcherBuilder(DataQueue *dataQ, String publishName, const std::vector<IntervalCommand*>& commands) {
    _dataQ = dataQ;
    _commands = commands;
    _publishName = publishName;

    // add command intervals to map and increment interval counts
    for (IntervalCommand* command : commands) {
        ++_intervalMap[command->getInterval()];
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
        for (IntervalCommand* command : _commands) {
            if (command->getInterval() == interval)
                commandsOnInterval[commandCount++] = command;
        }
        commandGroups[i++] = new IntervalCommandGroup(commandsOnInterval, numCommandsOnInterval, interval);
    }

    return new LoggingDispatcher(commandGroups, numIntervals, _dataQ, _publishName);
}