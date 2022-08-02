#ifndef _CAN_SENSOR_BMS_H_
#define _CAN_SENSOR_BMS_H_

#include "CanListener.h"
#include "CanInterface.h"
#include "BmsFault.h"

#define SOC_UPDATE_INTERVAL 497 // slighty off to avoid overlapping with other can write intervals

using namespace BmsFault;

/**
 * @brief Base bms class with abstract definitions of common bms properties and methods
 * 
 */
class CanSensorBms : public CanListener {
	public:
		// Constructors
		CanSensorBms(CanInterface& caninterface);

		CanSensorBms(CanInterface& caninterface, uint16_t id);

		~CanSensorBms();

		/**
		 * @brief Checks if any messages have been receieved within TIME_BEFORE_SLEEP
		 * sets _isAsleep to true if no messages have been received within time frame
		 * 
		 */
        void handle() override;

		/**
		 * @brief returns time (in ms) at which this bms was last updated
		 */
		uint64_t getLastUpdateTime();

		/**
		 * @brief if true, this bms will stop sending out can messages which compete with other bms instances
		 */
		void setIsAsleep(bool value);
		
		/**
         * @brief Get the battery voltage
         */
        virtual String getBatteryVolt(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the battery current
         */
        virtual String getBatteryCurrent(bool& valid = Sensor::dummy) = 0;

        /**
         * @brief Get the cell minimum voltage
         */
        virtual String getMinVolt(bool& valid = Sensor::dummy) = 0;

		/**
         * @brief Get the cell maximum voltage
         */
        virtual String getMaxVolt(bool& valid = Sensor::dummy) = 0;

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
         * @brief Send a restart message to the BMS
         */
        virtual void restart() = 0;

	protected:
        enum BmsStatus { Charging, Charged, Discharging, Regeneration, Idle, FaultError, ChargeEnabled, DischargeEnabled, Unknown };
		const char* BMS_STATUS_STRINGS[9] = { "Charging...", "Charged!", "Discharging...", "Regeneration", "Idle", "Fault Error", "Charge Enabled", "Discharge Enabled", "Unknown" };

		// Data
		float _batteryVoltage = 0.0f;
        float _batteryCurrent = 0.0f;
        float _cellVoltageMax = 0.0f;
        float _cellVoltageMin = 0.0f;
        float _soc = 0.0f;
        int _tempBms = 0;
        int _fault = NONE;
        BmsStatus _bmsStatus = Unknown;

		// Mangagement
		uint64_t _lastUpdateTime = 0;
		uint64_t _lastSocUpdate = 0;
		bool _isAsleep = false;
		std::unordered_map<uint16_t, uint64_t> _validationMap;

		/**
		 * @brief Validate current value based on value id
		 * 
		 * @param id numeric id of property to be validated
		 * @returns true if property is valid
		 */
        bool _validate(uint16_t id);

		/**
		 * @brief sends soc update out over can network on SOC_UPDATE_INTERVAL
		 */
		void _sendSocUpdate();
};

#endif