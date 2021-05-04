#ifndef _SENSOR_CAN_H_
#define _SENSOR_CAN_H_

#include "mcp_can.h"
#include "Sensor.h"

class SensorCan : public Sensor {
    public:
        SensorCan(SPIClass *spi, uint8_t csPin, uint8_t intPin);

        void begin();
        void handle();

        String getHumanName();
    

    private:
        SPIClass *_spi;
        uint8_t _csPin;
        uint8_t _intPin;

};

#endif
