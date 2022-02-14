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
        void begin() override;

        /**
         * Empty handle since signal strength and quality run on publish
        **/
        void handle() override;

        String getHumanName() override;

        /**
         * @return Signal strength in percentage
        **/
        int getStrength(); 

        /**
         * @return Signal quality in percentage
        **/
        int getQuality();


    
    private:
        CellularSignal _sig; //Object to call strength and quality commands
};

#endif