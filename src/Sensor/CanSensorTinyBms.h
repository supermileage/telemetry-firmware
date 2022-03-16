#ifndef _CAN_SENSOR_TINY_BMS_H_
#define _CAN_SENSOR_TINY_BMS_H_

#include <map>
#include "CanListener.h"
#include "BmsFault.h"

using namespace can;

class CanSensorTinyBms : public CanListener {
    public:
        enum BmsStatus { Charging, Charged, Discharging, Regeneration, Idle, FaultError, Unknown };
    
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
		 * @brief Sets a Can Callback message to be updated with voltage data
		 * 
		 */
		void setVoltageCallback(void (*callback)(float,float));

        /**
         * @brief Get the universal BMS fault code (if any)
         */
        int getFault(bool& valid = Sensor::dummy);

        /**
         * @brief Send a restart message to the BMS
         */
        void restart();

    private:
		// Control
        const uint16_t _requestIntervalMs;
        unsigned long _lastValidTime = 0;
        uint8_t _currentParam = 0;
        std::map<uint8_t, uint64_t> _validationMap;

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
        
		// Callback fn to pass current voltage to steering
		void (*_voltageCallback)(float,float) = NULL;

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
