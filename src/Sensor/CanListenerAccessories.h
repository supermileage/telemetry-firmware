#ifndef CAN_ACCESSORIES_LISTENER
#define CAN_ACCESSORIES_LISTENER

#include <array>

#include "can.h"
#include "CanListener.h"
#include "CanInterface.h"
#include "IntervalCommand.h"

using namespace can;

class CanListenerAccessories : public CanListener {
	public:
		/**
		 * @brief Construct for CanListenerAccessories
		 * 
		 * @param canInterface the can interface which will be reading data from Can buffer
		 * @param id the id of the Can message we will be reading from
		 * @param ids the individual ids of the Can accessories whose status we want (0xFF == unused)
		 */
		typedef std::array<uint8_t, 8> StatusIds;
		CanListenerAccessories(CanInterface& canInterface, uint16_t id, StatusIds ids);

		/**
		 * @brief Nothing to handle here
		 */
		void handle() { }

		/**
		 * @brief Get the string name of this object
		 */
		String getHumanName();
		
		/**
		 * @brief Get the status of urban headlights
		 */
		String getStatusHeadlights();

		/**
		 * @brief Get the status of urban brakelights
		 */
		String getStatusBrakelights();

		/**
		 * @brief Get the status of urban headlights
		 */
		String getStatusHorn();

		/**
		 * @brief Get the status of urban windshield wipers
		 */
		String getStatusHazards();

		/**
		 * @brief Get the status of urban windshield wipers
		 */
		String getStatusRightSignal();

		/**
		 * @brief Get the status of urban windshield wipers
		 */
		String getStatusLeftSignal();

		/**
		 * @brief Get the status of urban windshield wipers
		 */
		String getStatusWipers();


	private:
		StatusIds _idArray;
		CanMessage _statusMessage;

		/**
		 * @brief Get the status of 
		 * 
		 * @param statusId the id of the accessory you want to grab
		 */
		String _getStatus(uint8_t statusId);

		/**
		 * @brief adds data to appropriate byte of _statusMessage's data buffer
		 * 
		 * @param data data byte to be added to internal can message
		 */
		void _updateMessage(CanMessage message);

		/**
		 * @brief returns array index of statusId
		 * 
		 * @param statusId the status id whose index we want
		 */
		uint8_t _getCanMessageDataIndex(uint8_t statusId);
};

#endif