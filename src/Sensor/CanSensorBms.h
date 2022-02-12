#ifndef _SENSOR_BMS_H_
#define _SENSOR_BMS_H_

#include "CanListener.h"

#define NUM_PARAMS                  7

#define PARAM_ID_BATTERY_VOLTAGE    0x14
#define PARAM_ID_BATTERY_CURRENT    0x15
#define PARAM_ID_MAX_CELL_VOLTAGE   0x16
#define PARAM_ID_MIN_CELL_VOLTAGE   0x17
#define PARAM_ID_STATUS             0x18
#define PARAM_ID_SOC                0x1A
#define PARAM_ID_TEMP               0x1B

#define STATUS_CHARGING             0x91
#define STATUS_CHARGED              0x92
#define STATUS_DISCHARGING          0x93
#define STATUS_REGENERATION         0x96
#define STATUS_IDLE                 0x97
#define STATUS_FAULT_ERROR          0x9B

#define TEMP_ID_INTERNAL    0x00
#define TEMP_ID_BATTERY_1   0x01
#define TEMP_ID_BATTERY_2   0x02

#define REQ_DATA_LENGTH     8

#define RSP_STATUS_BYTE     0x0
#define RSP_PARAM_ID_BYTE   0x1
#define RSP_DATA_BYTE       0x2

using namespace can;

class CanSensorBms : public CanListener {
    public:
        enum BmsStatus { Charging, Charged, Discharging, Regeneration, Idle, FaultError, Unknown };
    
        /**
         * @brief Constructor for CanSensorBms
         * 
         * @param canInterface - the can interface which will be reading data from Can buffer
         * @param requestIntervalMs - frequency to request Bms data
         */
        CanSensorBms(CanInterface &canInterface, uint16_t requestIntervalMs);

        /**
         * @brief Repeatedly requests and stores bms data on interval
         */
        void handle();
        
        /**
         * @brief Get the string name of this object
         */
        String getHumanName();

        /**
         * @brief Get the battery voltage
         */
        String getBatteryVolt();

        /**
         * @brief Get the battery current
         */
        String getBatteryCurrent();

        /**
         * @brief Get the cell maximum voltage
         */
        String getMaxVolt();

        /**
         * @brief Get the cell minimum voltage
         */
        String getMinVolt();

        /**
         * @brief Get the battery state of charge
         */
        String getSoc();

        /**
         * @brief Get the current Bms status
         */
        int getStatusBms();

        /**
         * @brief Get current Bms status as string
         */
        String getStatusBmsString();

        /**
         * @brief Get the Bms internal temperature
         */
        int getTempBms();

        /**
         * @brief Get the Bank 1 battery temperature
         */
        int getBatteryTemp1();

        /**
         * @brief Get the Bank 2 battery temperature
         */
        int getBatteryTemp2();

    private:

        unsigned long _lastValidTime = 0;

        const uint16_t _requestIntervalMs;

        const uint8_t _paramIds[NUM_PARAMS] = 
            {   PARAM_ID_BATTERY_VOLTAGE, 
                PARAM_ID_BATTERY_CURRENT, 
                PARAM_ID_MAX_CELL_VOLTAGE, 
                PARAM_ID_MIN_CELL_VOLTAGE, 
                PARAM_ID_STATUS, 
                PARAM_ID_SOC, 
                PARAM_ID_TEMP};
        
        uint8_t _currentParam = 0;

        // Data
        float _batteryVoltage = 0.0f;
        float _batteryCurrent = 0.0f;
        float _cellVoltageMax = 0.0f;
        float _cellVoltageMin = 0.0f;
        float _soc = 0.0f;        
        int _tempBms = 0;
        int _batteryTemp1 = 0;
        int _batteryTemp2 = 0;
        BmsStatus _bmsStatus = Unknown;
        const char* bmsStatuses[7] = { "Charging...", "Charged!", "Discharging...", "Regeneration", "Idle", "Fault Error", "Unknown" };

        float parseFloat(uint8_t* dataPtr);

        uint16_t parseInt16(uint8_t* dataPtr);

        uint32_t parseInt32(uint8_t* dataPtr);

        /**
         * @brief Determines type of bms data and stores respectively
         * 
         * @param message data byte to be added to internal can message
         */
        void update(CanMessage message);
};

#endif
