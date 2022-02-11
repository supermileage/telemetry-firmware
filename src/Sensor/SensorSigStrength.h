#ifndef _SENSOR_SIG_H_
#define _SENSOR_SIG_H_

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
        int getStrength(bool &valid = Sensor::dummy); 

        /**
         * @return Signal quality in percentage
        **/
        int getQuality(bool &valid = Sensor::dummy);


    
    private:
        CellularSignal _sig; //Object to call strength and quality commands
};

#endif