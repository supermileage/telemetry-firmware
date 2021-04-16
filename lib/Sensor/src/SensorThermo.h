#include "Particle.h"
#include "Adafruit_MAX31855.h"

class SensorThermo {
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