#ifndef _DISPATCHER_BUILDER_H_
#define _DISPATCHER_BUILDER_H_

#include "DataQueue.h"
#include "IntervalCommand.h"
#include "Dispatcher.h"
#include "IntervalCommandGroup.h"

#define MAX_NUM_INTERVALS 5

// Add all the commands you want to this and call build().  It
// produces a dispatcher with all logging functionality already set up
// You can delete this safely after build() has been called
class DispatcherBuilder {
    public:
        /**
         * Constructs new DispatcherBuilder with number of commands and data queue
         * 
         * @param numCommands The number of sensor functions you will add to the builder
         * Note: this must be accurate and has potential to cause a segfault if the value is too low
         * @param dataQ the Data Queue from main
         * */
        DispatcherBuilder(IntervalCommand *commands[], uint16_t numCommands, DataQueue *dataQ);
        
        /**
         * @brief Destroy the Dispatcher Builder object
         */
        ~DispatcherBuilder();

        /**
         * Builds and returns a dispatcher with all of the logging information you've added
         **/
        Dispatcher* build();

    private:
        // adds interval to this builder's set of intervals
        void addInterval(IntervalCommand *command);

        // return true if array of intervals already contains this interval
        bool containsInterval(uint16_t newInterval);

        // keeps track of how many commands are on the same interval
        void updateIntervals(uint16_t newInterval);

        IntervalCommand **_commands;
        DataQueue *_dataQ;
        uint16_t *_intervals;
        uint16_t *_numCommandsAddedOnIntervals;
        uint16_t _numIntervals;
        uint16_t _numCommands;
};

#endif