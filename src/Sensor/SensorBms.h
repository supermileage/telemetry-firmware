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

using namespace can;

class SensorBms : public CanListener {
    public:

        /**
         * CONSTRUCTOR
         */
        SensorBms(CanInterface &canInterface, uint16_t requestIntervalMs);

        /**
         * 
         **/
        void handle();

        String getHumanName();

        void update(CanMessage message);

        float getBatteryVolt();

        float getBatteryCurrent();

        float getMaxVolt();

        float getMinVolt();

        float getSoc();

        String getStatusBms();

        int getTempBms();

        int getBatteryTemp1();

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

        // Data
        float _batteryVoltage;
        float _batteryCurrent;
        float _cellVoltageMax;
        float _cellVoltageMin;
        float _soc;
        String _bmsStatus;
        int _tempBms;
        int _batteryTemp1;
        int _batteryTemp2;

        float parseFloat(uint8_t* dataPtr);

};

#endif
