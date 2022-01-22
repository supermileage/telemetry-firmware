#ifndef _SENSOR_BMS_H_
#define _SENSOR_BMS_H_

#include "Sensor.h"

class SensorBms : public Sensor {
    public:

        /**
         * 
         **/
        void begin();

        /**
         * 
         **/
        void handle();

        String getHumanName();

        String getSoc();

        String getBatteryVolt();

        String getBatteryCurrent();

        String getMaxVolt();

        String getMinVolt();

        String getStatusBMS();

        int getTempBMS();

        int getBatteryTemp1();

        int getBatteryTemp2();

    private:

};

#endif
