#ifndef _CAN_SENSOR_BMS_H_
#define _CAN_SENSOR_BMS_H_

#include "CanListener.h"
#include "CanInterface.h"

/**
 * @brief Base bms class with abstract definitions of common bms properties and methods
 * 
 */
class CanSensorBms : public CanListener {
	public:
        enum BmsStatus { Charging, Charged, Discharging, Regeneration, Idle, FaultError, Unknown };

		// Constructors
		CanSensorBms(CanInterface& caninterface);

		CanSensorBms(CanInterface& caninterface, uint16_t id);

		~CanSensorBms();
		
		/**
         * @brief Get the battery voltage
         */
        String getBatteryVolt(bool& valid = Sensor::dummy);

        /**
         * @brief Get the battery current
         */
        String getBatteryCurrent(bool& valid = Sensor::dummy);

        /**
         * @brief Get the cell maximum voltage
         */
        String getMaxVolt(bool& valid = Sensor::dummy);

        /**
         * @brief Get the cell minimum voltage
         */
        String getMinVolt(bool& valid = Sensor::dummy);

        /**
         * @brief Get the battery state of charge
         */
        String getSoc(bool& valid = Sensor::dummy);

		/**
         * @brief Get the Bms internal temperature
         */
        int getTempBms(bool& valid = Sensor::dummy);

        /**
         * @brief Get the Bank 1 battery temperature
         */
        int getBatteryTemp1(bool& valid = Sensor::dummy);

        /**
         * @brief Get the Bank 2 battery temperature
         */
        int getBatteryTemp2(bool& valid = Sensor::dummy);

		/**
         * @brief Get the current Bms status
         */
        int getStatusBms(bool& valid = Sensor::dummy);

        /**
         * @brief Get current Bms status as string
         */
        String getStatusBmsString(bool& valid = Sensor::dummy);

		/**
         * @brief Get the universal BMS fault code (if any)
         */
        virtual int getFault(bool& valid = Sensor::dummy) = 0;
		
		/**
		 * @brief Sets a Can Callback message to be updated with voltage data
		 * 
		 */
		void setVoltageCallback(void (*callback)(float,float));

        /**
         * @brief Send a restart message to the BMS
         */
        virtual void restart() = 0;

	protected:
        // Data
        float _batteryVoltage = 0.0f;
        float _batteryCurrent = 0.0f;
        float _cellVoltageMax = 0.0f;
        float _cellVoltageMin = 0.0f;
		float _cellVoltageAvg = 0.0f;
        float _soc = 0.0f;
        int _tempBms = 0;
		int _batteryTemp1 = 0;
        int _batteryTemp2 = 0;
		int _batteryTempAvg = 0;
        uint8_t _fault = 0;
        BmsStatus _bmsStatus = Unknown;

		void (*_voltageCallback)(float,float) = NULL;
		std::map<uint8_t, uint64_t> _validationMap;

		/**
		 * @brief Validate current value based on value id
		 * 
		 * @param id numeric id of property to be validated
		 * @returns true if property is valid
		 */
        bool _validate(uint8_t id);
};

#endif