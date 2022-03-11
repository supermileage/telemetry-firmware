#ifndef _SENSOR_CAN_EXAMPLE_H_
#define _SENSOR_CAN_EXAMPLE_H_

#include "Sensor.h"
#include "CanInterface.h"
#include "can_common.h"

/**
 * @brief Base CanListener class for specifying parsing behavior of can messages with id.
 * update is called every time CanInterface receives a message with id
 * 
 * @note if for whatever reason you want to recieve can messages of multiple different ids, you will
 * need to override begin() in derived class and invoke CanInterface.addMessageListen multiple times
 * 
 */
class CanListener : public Sensor {

    public:
        /**
         * Constructor
         * 
         * @param canInterface can interface object
         * @param id the can id that this class will listen for
         **/
        CanListener(CanInterface &canInterface, uint16_t id) : _canInterface(canInterface), _id(id) { }

        /**
		 * @brief Called on setup: adds id and delegate to can interface
		 */
        virtual void begin() override;

        virtual void handle() = 0;

        virtual String getHumanName() = 0;

    protected:
        CanInterface &_canInterface;
        uint16_t _id;

    private:
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
				 * @param args cast CanMessage with this CanSensorAccessories's id
				 */
				void execute(CommandArgs args) override;
			private:
				CanListener* _owner;
		};

};

#endif
