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
         * @brief Get the battery voltage
         */
        String getBatteryVolt(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the battery current
         */
        String getBatteryCurrent(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the cell maximum voltage
         */
        String getMaxVolt(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the cell minimum voltage
         */
        String getMinVolt(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the battery state of charge
         */
        String getSoc(bool& valid = Sensor::dummy) override;

		/**
         * @brief Get the current Bms status
         */
        int getStatusBms(bool& valid = Sensor::dummy) override;

		/**
         * @brief Get current Bms status as string
         */
        String getStatusBmsString(bool& valid = Sensor::dummy) override;

		/**
         * @brief Get the Bms internal temperature
         */
        int getTempBms(bool& valid = Sensor::dummy) override;
		
		/**
		 * @brief Sets a Can Callback message to be updated with voltage data
		 * 
		 */
		void setVoltageCallback(void (*callback)(float,float)) override;

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