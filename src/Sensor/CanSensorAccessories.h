#ifndef CAN_ACCESSORIES_LISTENER
#define CAN_ACCESSORIES_LISTENER

#include <map>
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
class CanSensorAccessories : public CanListener {
	public:
		/**
		 * @brief return values for getStatus methods
		 */
		enum Status { Off = 0, On = 1, Unknown = 2 };

		/**
		 * @brief Constructor for CanSensorAccessories
		 * 
		 * @param canInterface the can interface which will be reading data from Can buffer
		 * @param id the id of the Can message this object will listen for
		 * @param ids the individual ids of the Can accessories whose status we want (0xFF == unused)
		 * @note size of ids array can be increased if we need to listen for more Can status messages
		 */
		typedef std::array<uint8_t, 7> StatusIds;
		CanSensorAccessories(CanInterface &canInterface, uint16_t id, StatusIds ids);

		/**
		 * @brief Nothing to handle here
		 */
		void handle() override { }

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
		std::map<uint8_t, uint8_t> _statuses;

		/**
		 * @brief adds data to appropriate byte of _data's data buffer
		 * 
		 * @param data data byte to be added to internal can message
		 */
		void update(CanMessage message) override;
};

#endif