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

        float getSoc();

    private:


};

#endif
