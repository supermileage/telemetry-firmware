#ifndef _SENSOR_FC_H_
#define _SENSOR_FC_H_

#include "Sensor.h"

#define FC_BUFFER_SIZE 512

class SensorFc : public Sensor {
    public:
        /**
         * Constructor 
         * @param *serial bus receiving Fuel Cell Controller data
         **/
        SensorFc(USARTSerial *serial);

        /**
         * 
         * */
        void begin() override;
        
        /**
         * 
         * */
        void handle() override;

        String getHumanName() override;


    private:
        USARTSerial * _serial;
        char _buffer[FC_BUFFER_SIZE];
        uint32_t _lastUpdate = 0;
};

#endif
