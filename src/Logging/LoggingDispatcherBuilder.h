#ifndef _DISPATCHER_BUILDER_H_
#define _DISPATCHER_BUILDER_H_

#include <unordered_map>
#include <vector>

#include "DataQueue.h"
#include "IntervalCommand.h"
#include "LoggingDispatcher.h"
#include "IntervalCommandGroup.h"

/**
 * Add all the commands you want to this and call build().  Produces LoggingDispatcher, which will execute commands on their specified intervals
 **/
class LoggingDispatcherBuilder {
    public:
        /**
         * Constructs new LoggingDispatcherBuilder with number of commands and data queue
         *
         * @param numCommands The number of sensor functions you will add to the builder
         * Note: this must be accurate and has potential to cause a segfault if the value is too low
         * @param dataQ the Data Queue from main
         * @param publishName the name under which any data from interval commands will be published
         **/
        LoggingDispatcherBuilder(DataQueue *dataQ, String publishName, const std::vector<IntervalCommand*>& commands);
        
        /**
         * @brief Destroy the LoggingDispatcher Builder object
         */
        ~LoggingDispatcherBuilder();

        /**
         * Builds and returns a dispatcher with all of the logging information you've added
         **/
        LoggingDispatcher* build();

    private:
        std::vector<IntervalCommand*> _commands;
        DataQueue* _dataQ;
        std::unordered_map<uint16_t, uint16_t> _intervalMap;
        String _publishName;
};

#endif