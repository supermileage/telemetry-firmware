#ifndef CAN_ACCESSORIES_LISTENER
#define CAN_ACCESSORIES_LISTENER

#include <array>

#include "SensorCanBase.h"
#include "CanInterface.h"
#include "Command.h"

class CanAccessoriesListener : public SensorCanBase {
	public:
		/**
		 * @brief Construct for CanAccessoriesListener
		 * 
		 * @param canInterface the can interface which will be reading data from Can buffer
		 * @param id the id of the Can message we will be reading from
		 * @param ids the individual ids of the Can accessories whose status we want
		 */
		typedef std::array<uint8_t, 8> StatusIds;
		CanAccessoriesListener(CanInterface& canInterface, uint16_t id, StatusIds ids);

		/**
		 * @brief Called on setup: adds id and delegate to can interface
		 */
		void begin();

		void handle() { }

		/**
		 * @brief Get the string name of this object
		 */
		String getHumanName();
		
		/**
		 * @brief Get the status of can accessory with id
		 * 
		 * @param statusId the id of the accessory you want to grab
		 */
		String getStatus(uint8_t statusId);

	private:
		StatusIds _idArray;
		CanInterface::CanMessage _statusMessage;

		/**
		 * @brief adds data to appropriate byte of _statusMessage's data buffer
		 * 
		 * @param data data byte to be added to internal can message
		 */
		void _updateMessage(uint8_t data);

		/**
		 * @brief returns array index of statusId
		 * 
		 * @param statusId the status id whose index we want
		 */
		uint8_t _getCanMessageDataIndex(uint8_t statusId);

		/**
		 * Internal class which acts as a delegate to CanInterface; allows us to update _statusMessage
		 *  without overwriting data that we still need
		 */
		class CanAccessoriesMessageParser : public Command {
			public:
				CanAccessoriesMessageParser(CanAccessoriesListener* owner) : _owner(owner) { }

				~CanAccessoriesMessageParser() { }

				/**
				 * Delegate function which will be called in CanInterface
				 * parse can data and adds to _statusMessage
				 * 
				 * @param arg can message with this CanAccessoriesListener's id
				 */
				void* execute(void* arg);
			private:
				CanAccessoriesListener* _owner;
		};
};

#endif