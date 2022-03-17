#ifndef _CAN_SENSOR_ORION_BMS_H_
#define _CAN_SENSOR_ORION_BMS_H_

#include "CanSensorBms.h"
#include "CanInterface.h"

class CanSensorOrionBms : public CanSensorBms {
	public:
		CanSensorOrionBms(CanInterface& canInterface);

		~CanSensorOrionBms();

        /**
		 * @brief Called on startup: adds Orion Can message ids to CanInterface object
		 */
        void begin() override;

		/**
		 * @brief Does nothing
		 * 
		 */
        void handle() override;

		/**
		 * @brief Get the Human Name of CanOrionBms
		 */
        String getHumanName() override;

        /**
         * @brief Get the universal BMS fault code (if any)
         */
        int getFault(bool& valid = Sensor::dummy) override;

        /**
         * @brief Send a restart message to the BMS
         */
        void restart() override;

	private:
		/**
         * @brief Called by delegate in CanInterface when a message with one of Orion's ids is receieved
         * 
         * @param message CanMessage received from CanInterface with OrionBms data
         */
		void update(CanMessage message) override;

		/**
		 * @brief parse a big-endian two byte signed integer from buffer
		 * 
		 * @param buf pointer to first byte to be read (left to right)
		 */
		int16_t parseInt16(uint8_t* buf);
};

#endif