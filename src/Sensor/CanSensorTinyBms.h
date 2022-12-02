#ifndef _CAN_SENSOR_TINY_BMS_H_
#define _CAN_SENSOR_TINY_BMS_H_

#include <unordered_map>
#include "CanSensorBms.h"
#include "BmsFault.h"

#define TINYBMS_NUM_PARAMS                          7
#define TINYBMS_REQ_DATA_LENGTH     		        8

#define TINYBMS_PARAM_ID_BATTERY_VOLTAGE            0x14
#define TINYBMS_PARAM_ID_BATTERY_CURRENT            0x15
#define TINYBMS_PARAM_ID_MAX_CELL_VOLTAGE           0x16
#define TINYBMS_PARAM_ID_MIN_CELL_VOLTAGE           0x17
#define TINYBMS_PARAM_ID_STATUS                     0x18
#define TINYBMS_PARAM_ID_SOC                        0x1A
#define TINYBMS_PARAM_ID_TEMP                       0x1B

#define TINYBMS_PARAM_ID_EVENTS                     0x11
#define TINYBMS_PARAM_ID_RESET                      0x02
#define TINYBMS_RESET_ID_BMS                        0x05

#define TINYBMS_STATUS_CHARGING                     0x91
#define TINYBMS_STATUS_CHARGED                      0x92
#define TINYBMS_STATUS_DISCHARGING                  0x93
#define TINYBMS_STATUS_REGENERATION                 0x96
#define TINYBMS_STATUS_IDLE                         0x97
#define TINYBMS_STATUS_FAULT_ERROR                  0x9B

#define TINYBMS_TEMP_ID_INTERNAL    		        0x00
#define TINYBMS_TEMP_ID_BATTERY_1   		        0x01
#define TINYBMS_TEMP_ID_BATTERY_2   		        0x02

#define TINYBMS_RSP_STATUS_BYTE     		        0x0
#define TINYBMS_RSP_PARAM_ID_BYTE   		        0x1
#define TINYBMS_RSP_DATA_BYTE       		        0x2
#define TINYBMS_TEMP_ID_BYTE						0x5

#define TINYBMS_FAULT_UNDER_VOLTAGE                 0x02
#define TINYBMS_FAULT_OVER_VOLTAGE                  0x03
#define TINYBMS_FAULT_OVER_TEMP                     0x04
#define TINYBMS_FAULT_OVER_CURRENT_DISCHARGE        0x05
#define TINYBMS_FAULT_OVER_CURRENT_CHARGE           0x06
#define TINYBMS_FAULT_LOW_TEMP                      0x0A
#define TINYBMS_FAULT_CHARGER_SWITCH                0x0B
#define TINYBMS_FAULT_LOAD_SWITCH                   0x0C
#define TINYBMS_FAULT_SINGLE_PORT_SWITCH            0x0D
#define TINYBMS_FAULT_CURRENT_SENSOR_DISCONNECTED   0x0E
#define TINYBMS_FAULT_CURRENT_SENSOR_CONNECTED      0x0F

#define TINYBMS_TEMP_SCALING_FACTOR					10

using namespace can;

class CanSensorTinyBms : public CanSensorBms {
    public:
		static const uint8_t* ParamIds;

        /**
         * @brief Constructor for CanSensorTinyBms
         * 
         * @param canInterface - the can interface which will be reading data from Can buffer
         * @param requestIntervalMs - frequency to request Bms data
         */
        CanSensorTinyBms(CanInterface &canInterface, uint16_t requestIntervalMs);

		/**
         * @brief Repeatedly requests and stores bms data on interval
         */
        void handle() override;

		/**
		 * @brief Calls CanListener::begin and initializes validation map
		 */
		void begin() override;

		/**
         * @brief Get the string name of this object
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
         * @brief Get the cell minimum voltage
         */
        String getMinVolt(bool& valid = Sensor::dummy) override;

		/**
         * @brief Get the cell maximum voltage
         */
        String getMaxVolt(bool& valid = Sensor::dummy) override;

		/**
         * @brief Get the cells' average voltage
         */
		String getAvgVolt(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the battery state of charge
         */
        String getSoc(bool& valid = Sensor::dummy) override;

		/**
         * @brief Get the Bms internal temperature
         */
        int getTempBms(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the min temp of all battery packs
         */
        int getMinBatteryTemp(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the max temp of all battery packs
         */
        int getMaxBatteryTemp(bool& valid = Sensor::dummy) override;

		/**
		 * @brief Get the average battery temperation of all packs
		 */
		int getAvgBatteryTemp(bool& valid = Sensor::dummy) override;

		/**
         * @brief Get the current Bms status
         */
        int getStatusBms(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get current Bms status as string
         */
        String getStatusBmsString(bool& valid = Sensor::dummy) override;

        /**
         * @brief Get the universal BMS fault code (if any)
         */
        int getFault(bool& valid = Sensor::dummy) override;

        /**
         * @brief Send a restart message to the BMS
         */
        void restart() override; 

    private:
		// Control
        const uint16_t _requestIntervalMs;
        unsigned long _lastValidTime = 0;
        uint8_t _currentParam = 0;
		int _batteryTemp1 = 0;
		int _batteryTemp2 = 0;

        /**
         * @brief Parse number in TinyBMS Float format
         * 
         * @param dataPtr Base address of data
         */
        float parseFloat(uint8_t* dataPtr);

        /**
         * @brief Parse number in TinyBMS 16-bit int format
         * 
         * @param dataPtr Base address of data
         */
        int16_t parseInt16(uint8_t* dataPtr);

        /**
         * @brief Parse number in TinyBMS 32-bit int format
         * 
         * @param dataPtr Base address of data
         */
        int32_t parseInt32(uint8_t* dataPtr);

        /**
         * @brief Determines type of bms data and stores respectively
         * 
         * @param message data byte to be added to internal can message
         */
        void update(CanMessage message) override;

        /**
         * @brief Converts TinyBMS fault code into universal fault code
         * 
         * @param fault TinyBMS fault code
         */
        uint8_t _getFaultCode(uint8_t fault);
};

#endif
