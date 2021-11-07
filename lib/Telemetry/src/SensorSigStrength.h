#ifndef _SENSOR_SIG_H_
#define _SENSOR_CAN_H_

#include "Particle.h"
#include "Sensor.h"

class SensorSigStrength : public Sensor
{
    public:

        /**
         * Constructor
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

        /**
         * @return Signal strength in percentage
        **/
        String getStrength(); 

        /**
         * @return Signal quality in percentage
        **/
        String getQuality();


    
    private:
        CellularSignal _sig; //Object to call strength and quality commands
};

#endif