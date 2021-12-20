#ifndef CAN_ACCESSORIES_LISTENER
#define CAN_ACCESSORIES_LISTENER

#include <array>
#include <bitset>

#include "SensorCanBase.h"
#include "CanInterface.h"
#include "IntervalCommand.h"

class CanAccessoriesListener : public SensorCanBase {
	public:
		/**
		 * @brief Construct for CanAccessoriesListener
		 * 
		 * @param canInterface the can interface which will be reading data from Can buffer
		 * @param id the id of the Can message we will be reading from
		 * @param ids the individual ids of the Can accessories whose status we want (0xFF == unused)
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

		/**
		 * @brief Logging command for 
		 * 
		 */
		class LoggingCommand : public IntervalCommand {
			public:
				LoggingCommand(CanAccessoriesListener* owner, String propertyName, uint16_t id, uint16_t interval)
				: IntervalCommand(interval) {
					_owner = owner;
					_propertyName = propertyName;
					_id = id;
				}

				~LoggingCommand() { }

				/**
				 * @brief logs can data to data queue
				 * 
				 * @param args DataQueue which will be used to log status for _id
				 * @return void* 
				 */
				void* execute(CommandArgs args);
			private:
				CanAccessoriesListener* _owner;
				String _propertyName;
				uint16_t _id;
		};

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
		 * Internal class which acts as a delegate to CanInterface; allows us to update member
		 * CanMessage without overwriting data that we still need
		 */
		class CanMessageParser : public Command {
			public:
				CanMessageParser(CanAccessoriesListener* owner) : _owner(owner) { }

				~CanMessageParser() { }

				/**
				 * Delegate function which will be called in CanInterface:
				 * parses can data and adds to CanListener's member CanMessage
				 * 
				 * @param arg can message with this CanAccessoriesListener's id
				 */
				void* execute(CommandArgs args);
			private:
				CanAccessoriesListener* _owner;
		};
};

#endif