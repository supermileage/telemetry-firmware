#ifndef _SENSOR_SIG_H_
#define _SENSOR_SIG_H_

#include "Particle.h"
#include "Sensor.h"

class SensorSigStrength : public Sensor
{
    public:

        /**
         * Constructor
         * @param /No external dependencies
        **/
        SensorSigStrength();

        /**
         *Empty handle since signal strength and quality run on publish
        **/
        void begin();

        /**
         * Empty handle since signal strength and quality run on publish
        **/
        void handle();

        String getHumanName();

        float getStrength(); 
        float getQuality();

        /**
         * @return Signal strength and quality in percentage
        **/
    
    private:
        CellularSignal _sig; //Object to call strength and quality commands
};

#endif