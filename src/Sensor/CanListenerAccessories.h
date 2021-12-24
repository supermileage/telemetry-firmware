#ifndef CAN_ACCESSORIES_LISTENER
#define CAN_ACCESSORIES_LISTENER

#include <array>

#include "can.h"
#include "CanListener.h"
#include "CanInterface.h"
#include "IntervalCommand.h"

using namespace can;

/**
 * @brief Class which stores and updates status for Can accessories
 * 
 */
class CanListenerAccessories : public CanListener {
	public:
		/**
		 * @brief return values for getStatus methods
		 */
		enum Status { Off = 0, On = 1, Unknown = 2 };

		/**
		 * @brief Construct for CanListenerAccessories
		 * 
		 * @param canInterface the can interface which will be reading data from Can buffer
		 * @param id the id of the Can message we will be reading from
		 * @param ids the individual ids of the Can accessories whose status we want (0xFF == unused)
		 */
		typedef std::array<uint8_t, 8> StatusIds;
		CanListenerAccessories(CanInterface* canInterface, uint16_t id, StatusIds ids);

		/**
		 * @brief Nothing to handle here
		 */
		void handle() { }

		/**
		 * @brief Get the string name of this object
		 */
		String getHumanName();
		
		/**
		 * @brief Get the status of headlights
		 */
		int getStatusHeadlights();

		/**
		 * @brief Get the status of brakelights
		 */
		int getStatusBrakelights();

		/**
		 * @brief Get the status of headlights
		 */
		int getStatusHorn();

		/**
		 * @brief Get the status of hazard lights
		 */
		int getStatusHazards();

		/**
		 * @brief Get the status of right signal
		 */
		int getStatusRightSignal();

		/**
		 * @brief Get the status of left signal
		 */
		int getStatusLeftSignal();

		/**
		 * @brief Get the status of windshield wipers
		 */
		int getStatusWipers();


	private:
		StatusIds _idArray;
		CanMessage _statusMessage;

		/**
		 * @brief Get the status of 
		 * 
		 * @param statusId the id of the accessory you want to grab
		 */
		int _getStatus(uint8_t statusId);

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