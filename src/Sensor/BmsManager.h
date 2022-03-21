#ifndef _BMS_MANAGER_H_
#define _BMS_MANAGER_H_

#include "CanSensorBms.h"

/**
 * @brief Class whose sole purpose is to automatically switch between tiny and orion bms systems when they are swapped out
 */
class BmsManager : public Handleable {
	public:
		enum BmsOptions { Orion, Tiny, None };
		
        /**
         * Constructor
         * 
         * Consumes pointer to main bms pointer, and pointer references to each of two bms types in project
         * */
        BmsManager(CanSensorBms** bms, CanSensorBms* orion, CanSensorBms* tiny, BmsOptions option);

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
		 * @brief Sets the current active bms
		 * 
		 * @param option enum for type to set bms to
		 */
		void setBms(BmsOptions option);

		/**
		 * @brief Returns name of active bms object or None if bms is null
		 */
		String getCurrentBmsName();

	private:
		CanSensorBms** _bms;
		CanSensorBms* _orion;
		CanSensorBms* _tiny;
		BmsOptions _currentBms = None;
		uint64_t _lastTime = 0;
};

#endif