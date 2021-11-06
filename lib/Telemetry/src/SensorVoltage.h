#ifndef _SENSOR_VOL_H_
#define _SENSOR_VOL_H_

#include "Particle.h"
#include "Sensor.h"

class SensorVoltage : public Sensor
{
    public:

        /**
         * Constructor 
         * @param /No external dependencies
        **/
        SensorVoltage();

        /**
         * Empty handle since input voltage run on publish
        **/
        void begin();

        /**
         * Empty handle since input voltage run on publish
        **/
        void handle();

        String getHumanName();

        float getVoltage();

        /**
         * @return //input voltage of Boron device
        **/
};

#endif