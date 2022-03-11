#ifndef _SENSOR_BMS_H_
#define _SENSOR_BMS_H_

#include <map>
#include "CanListener.h"
#include "BmsFault.h"

#define NUM_PARAMS                  7

#define PARAM_ID_BATTERY_VOLTAGE    0x14
#define PARAM_ID_BATTERY_CURRENT    0x15
#define PARAM_ID_MAX_CELL_VOLTAGE   0x16
#define PARAM_ID_MIN_CELL_VOLTAGE   0x17
#define PARAM_ID_STATUS             0x18
#define PARAM_ID_SOC                0x1A
#define PARAM_ID_TEMP               0x1B

#define PARAM_ID_EVENTS             0x11
#define PARAM_ID_RESET              0x02
#define RESET_ID_BMS                0x05

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
        void handle() override;
        
        /**
         * @brief Get the string name of this object
         */
        String getHumanName() override;

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
         * @brief Get the current Bms status
         */
        int getStatusBms(bool& valid = Sensor::dummy);

        /**
         * @brief Get current Bms status as string
         */
        String getStatusBmsString(bool& valid = Sensor::dummy);

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
         * @brief Get the universal BMS fault code (if any)
         */
        int getFault(bool& valid = Sensor::dummy);

        /**
         * @brief Send a restart message to the BMS
         */
        void restart();

    private:
        const uint16_t _requestIntervalMs;
        const uint8_t _paramIds[NUM_PARAMS] =  {
            PARAM_ID_BATTERY_VOLTAGE,
            PARAM_ID_BATTERY_CURRENT,
            PARAM_ID_MAX_CELL_VOLTAGE,
            PARAM_ID_MIN_CELL_VOLTAGE,
            PARAM_ID_STATUS,
            PARAM_ID_SOC,
            PARAM_ID_TEMP
        };
        const char* bmsStatuses[7] = { "Charging...", "Charged!", "Discharging...", "Regeneration", "Idle", "Fault Error", "Unknown" };
        std::map<uint8_t, uint64_t> _validationMap;
        unsigned long _lastValidTime = 0;
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
        uint8_t _fault = 0;

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
        uint16_t parseInt16(uint8_t* dataPtr);

        /**
         * @brief Parse number in TinyBMS 32-bit int format
         * 
         * @param dataPtr Base address of data
         */
        uint32_t parseInt32(uint8_t* dataPtr);

        bool _validate(uint8_t id);

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
