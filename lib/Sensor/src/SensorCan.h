#ifndef _SENSOR_CAN_H_
#define _SENSOR_CAN_H_

#include "ArduinoMCP2515.h"
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

        void spi_select();
        void spi_deselect();
        uint8_t spi_transfer(uint8_t const);
        void onExternalEvent();
        void onReceiveBufferFull(uint32_t const, uint32_t const, uint8_t const *, uint8_t const);

        ArduinoMCP2515 mcp2515(spi_select,
                       spi_deselect,
                       spi_transfer,
                       micros,
                       onReceiveBufferFull,
                       nullptr);

};

#endif
