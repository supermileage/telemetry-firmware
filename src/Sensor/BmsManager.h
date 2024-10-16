#ifndef _BMS_MANAGER_H_
#define _BMS_MANAGER_H_

#include "CanSensorBms.h"

/**
 * @brief Class whose sole purpose is to automatically switch between orion [and possibly new] bms systems when they are swapped out
 */
class BmsManager : public Sensor {
	public:
		static const uint32_t UpdateInterval;
		static const uint32_t MillisecondsBeforeDeselect;
		enum BmsOption { Orion, None };
		
        /**
         * Constructor
         * 
         * Consumes pointer reference to main bms pointer, and addresses of each of two bms instances in project
         * */
        BmsManager(CanSensorBms** bmsPtr, CanSensorBms* orion, BmsOption option);

        ~BmsManager();

        /**
         * Does nothing
         * */
        void begin() override;

        /**
         * Redirects primary bms pointer to whichever has most recently received messages
         * */
        void handle() override;

		/**
		 * @brief Get the string name of this object
		 */
		String getHumanName() override;

		/**
		 * @brief Sets the current active bms
		 * 
		 * @param option enum for type to set bms to
		 */
		void setBms(BmsOption option);

		/**
		 * @brief Returns name of active bms object or None if bms isn't assigned
		 * 
		 * @note unlike getCurrentBmsName, this method will return None if we neither bms has been updated
		 * within a certain time frame
		 */		
		int getCurrentBms(bool& valid = Sensor::dummy);

		/**
		 * @brief Returns name of active bms object or None if bms isn't assigned
		 */
		String getCurrentBmsName();

	private:
		CanSensorBms** _mainBmsPtr;
		CanSensorBms* _orion;
		BmsOption _currentOption = None;
		uint64_t _lastTime = 0;
};

#endif