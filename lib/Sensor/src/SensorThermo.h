#ifndef _SENSOR_THERMO_H_
#define _SENSOR_THERMO_H_

#include "Particle.h"
#include "Adafruit_MAX31855.h"
#include "Sensor.h"

class SensorThermo : public Sensor {
    public:
        SensorThermo(SPIClass *spi, uint8_t csPin, uint16_t updateInterval);

        void begin();
        void handle();

        double getTemp();

    private:
        Adafruit_MAX31855* _probe;
        SPIClass *_spi;
        uint16_t _updateInterval;
        unsigned long int _lastUpdate;
        double _temp;
};

#endif
