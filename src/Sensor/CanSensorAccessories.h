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
		CanSensorAccessories(CanInterface &canInterface, uint16_t id);

		/**
		 * @brief Nothing to handle here
		 */
		void handle() override { }

		/**
		 * @brief Calls CanListener::begin and initializes status map
		 */
		void begin() override;

		/**
		 * @brief Get the string name of this object
		 */
		String getHumanName();
		
		/**
		 * @brief Get the status of headlights
		 */
		int getStatusHeadlights(bool& valid = Sensor::dummy);

		/**
		 * @brief Get the status of brakelights
		 */
		int getStatusBrakelights(bool& valid = Sensor::dummy);

		/**
		 * @brief Get the status of headlights
		 */
		int getStatusHorn(bool& valid = Sensor::dummy);

		/**
		 * @brief Get the status of hazard lights
		 */
		int getStatusHazards(bool& valid = Sensor::dummy);

		/**
		 * @brief Get the status of right signal
		 */
		int getStatusRightSignal(bool& valid = Sensor::dummy);

		/**
		 * @brief Get the status of left signal
		 */
		int getStatusLeftSignal(bool& valid = Sensor::dummy);

		/**
		 * @brief Get the status of windshield wipers
		 */
		int getStatusWipers(bool& valid = Sensor::dummy);

	private:
		// struct to store status and validation data for accessories
		struct StatusProperty {
            unsigned long lastUpdated;
            int value;
        };
		std::map<uint8_t, StatusProperty> _statuses;

		/**
		 * @brief adds data to appropriate byte of _data's data buffer
		 * 
		 * @param data data byte to be added to internal can message
		 */
		void update(CanMessage message) override;

		/**
		 * @brief Internal get status method
		 * 
		 */
		int _getStatus(uint8_t id, bool& valid);
};

#endif