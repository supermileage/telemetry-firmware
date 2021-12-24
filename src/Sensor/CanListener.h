#ifndef _SENSOR_CAN_EXAMPLE_H_
#define _SENSOR_CAN_EXAMPLE_H_

#include "Sensor.h"
#include "CanInterface.h"
#include "can_common.h"

class CanListener : public Sensor {

    public:
        /**
         * Constructor
         * 
         * @param &can object to use
         **/
        CanListener(CanInterface* canInterface, uint16_t id) : _canInterface(canInterface), _id(id) { }

        /**
		 * @brief Called on setup: adds id and delegate to can interface
		 */
        virtual void begin();

        virtual void handle() = 0;

        virtual String getHumanName() = 0;

    protected:
        CanInterface* _canInterface;
        uint16_t _id;

        /**
         * @brief Specifies CanMessage updating behavior
         * 
         * @param message CanMessage received from CanInterface
         */
        virtual void _updateMessage(CanMessage message) = 0;

        /**
		 * CanListener-internal class which acts as a delegate to CanInterface; allows us to specify parsing
         * behavior in CanInterface without it needing to know about implementation of its listeners
		 */
		class CanListenerDelegate : public Command {
			public:
				CanListenerDelegate(CanListener* owner) : _owner(owner) { }

				~CanListenerDelegate() { }

				/**
				 * Delegate function which will be called in CanInterface:
				 * parses can data and adds to CanListener's member CanMessage
				 * 
				 * @param args can message with this CanListenerAccessories's id
				 */
				void execute(CommandArgs args);
			private:
				CanListener* _owner;
		};

};

#endif
