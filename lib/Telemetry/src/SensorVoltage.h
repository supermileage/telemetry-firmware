#ifndef _SENSOR_VOL_H_
#define _SENSOR_VOL_H_

#include "Particle.h"
#include "Sensor.h"

#define INPUT_VOLTAGE_PIN A1
#define ANALOG_CONVERT_RAW_TO_3V 1230
#define R1 10000
#define R2 2200

class SensorVoltage : public Sensor
{
    public:

        /**
         * Constructor 
        **/
        SensorVoltage();

        void begin();

        void handle();

        String getHumanName();

        /**
         * @return //input voltage of Boron device
        **/
        String getVoltage();

        
};

#endif
