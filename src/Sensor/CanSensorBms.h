#ifndef _CAN_SENSOR_BMS_H_
#define _CAN_SENSOR_BMS_H_

#include "CanListener.h"
#include "CanInterface.h"
#include "BmsFault.h"

using namespace BmsFault;

/**
 * @brief Base bms class with abstract definitions of common bms properties and methods
 * 
 */
class CanSensorBms : public CanListener {
	public:
        enum BmsStatus { Charging, Charged, Discharging, Regeneration, Idle, FaultError, ChargeEnabled, DischargeEnabled, Unknown };

		// Constructors
		CanSensorBms(CanInterface& caninterface);

		CanSensorBms(CanInterface& caninterface, uint16_t id);

		~CanSensorBms();
		
		/**
         * @brief Get the battery voltage
         */
        virtual String getBatteryVolt(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the battery current
         */
        virtual String getBatteryCurrent(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the cell maximum voltage
         */
        virtual String getMaxVolt(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the cell minimum voltage
         */
        virtual String getMinVolt(bool& valid = Sensor::dummy) = 0;

		/**
         * @brief Get the cells' average voltage
         */
		virtual String getAvgVolt(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the battery state of charge
         */
        virtual String getSoc(bool& valid = Sensor::dummy) = 0;

		/**
         * @brief Get the Bms internal temperature
         */
        virtual int getTempBms(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the max temp of all battery packs
         */
        virtual int getMinBatteryTemp(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the min temp of all battery packs
         */
        virtual int getMaxBatteryTemp(bool& valid = Sensor::dummy) = 0;

		/**
		 * @brief Get the average battery temperation of all packs
		 */
		virtual int getAvgBatteryTemp(bool& valid = Sensor::dummy) = 0;

		/**
         * @brief Get the current Bms status
         */
        virtual int getStatusBms(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get current Bms status as string
         */
        virtual String getStatusBmsString(bool& valid = Sensor::dummy) = 0;

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
		const char* BMS_STATUS_STRINGS[9] = { "Charging...", "Charged!", "Discharging...", "Regeneration", "Idle", "Fault Error", "Charge Enabled", "Discharge Enabled", "Unknown" };

		float _batteryVoltage = 0.0f;
        float _batteryCurrent = 0.0f;
        float _cellVoltageMax = 0.0f;
        float _cellVoltageMin = 0.0f;
		float _cellVoltageAvg = 0.0f;
        float _soc = 0.0f;
        int _tempBms = 0;
		int _batteryTempMax = 0;
        int _batteryTempMin = 0;
		int _batteryTempAvg = 0;
        int _fault = NONE;
        BmsStatus _bmsStatus = Unknown;

		void (*_voltageCallback)(float,float) = NULL;
		std::map<uint16_t, uint64_t> _validationMap;

		/**
		 * @brief Validate current value based on value id
		 * 
		 * @param id numeric id of property to be validated
		 * @returns true if property is valid
		 */
        bool _validate(uint16_t id);
};

#endif