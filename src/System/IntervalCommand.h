#ifndef _INTERVAL_COMMAND_H_
#define _INTERVAL_COMMAND_H_

#include <stdint.h>
#include <vector>

#include "Command.h"

class IntervalCommand : public Command {
    public:
        /**
         * Default constructor
         **/
        IntervalCommand() { }

        /**
         * Constructs IntervalCommand with interval 
         **/
        IntervalCommand(uint16_t interval);

        virtual ~IntervalCommand() { }

        /**
         * Virtual function executes the instruction owned by this command
         **/
        virtual void execute(CommandArgs args) = 0;

        /**
         *  Returns interval assigned to this command (_interval is only assigned in constructor of subclasses)
         **/
        uint16_t getInterval();

        /**
         * @brief Static method returns vector of IntervalCommand pointers
         */
        static const std::vector<IntervalCommand*>& getCommands();

    protected:
        uint16_t _interval;

        /**
         * @brief Internal class which acts as static collection of initialized interval commands
         */
        class IntervalCommandHandler {
            public:
                /**
                 * @brief singleton instance getter
                 * 
                 * @return IntervalCommandHandler& reference to singleton instance
                 */
                static IntervalCommandHandler& instance();

                /**
                 * @brief Adds IntervalCommand to collection
                 * 
                 * @param command pointer to IntervalCommand
                 */
                void add(IntervalCommand* command);

                /**
                 * @brief Getter for IntervalCommand collection
                 * 
                 * @return constant reference to IntervalCommand vector 
                 */
                const std::vector<IntervalCommand*>& getCommands();

            protected:
                static IntervalCommandHandler* _instance;
                std::vector<IntervalCommand*> _commands;
        };
};

#endif