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

        String getBatteryVolt();

        String getBatteryCurrent();

        String getMaxVolt();

        String getMinVolt();

        String getStatusBMS();

        String getSoc();

        int getTempBMS();

        int getBatteryTemp1();

        int getBatteryTemp2();

    private:

        unsigned long _lastValidTime = 0;

        const uint16_t _requestIntervalMs;

        const uint8_t _paramIds[NUM_PARAMS] = 
            {   PARAM_ID_PACK_VOLTAGE, 
                PARAM_ID_PACK_CURRENT, 
                PARAM_ID_MAX_CELL_VOLTAGE, 
                PARAM_ID_MIN_CELL_VOLTAGE, 
                PARAM_ID_STATUS, 
                PARAM_ID_SOC, 
                PARAM_ID_TEMP};

        

        
};

#endif
