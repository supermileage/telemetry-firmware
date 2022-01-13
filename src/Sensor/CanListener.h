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
        virtual void update(CanMessage message) = 0;

        /**
		 * CanListener-internal class which acts as a delegate to CanInterface; allows
         * CanListener-derived classes to parse CanMessages received in CanInterface
		 */
		class CanListenerDelegate : public Command {
			public:
				CanListenerDelegate(CanListener* owner) : _owner(owner) { }

				~CanListenerDelegate() { }

				/**
				 * Delegate function which will be called in CanInterface:
				 * takes CanMessage* as argument and passes it to update
				 * 
				 * @param args cast CanMessage with this CanListenerAccessories's id
				 */
				void execute(CommandArgs args);
			private:
				CanListener* _owner;
		};

};

#endif
