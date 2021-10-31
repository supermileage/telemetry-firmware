#ifndef _SENSOR_THERMO_H_
#define _SENSOR_THERMO_H_

#include "Particle.h"
#include "Adafruit_MAX31855.h"
#include "Sensor.h"

class SensorThermo : public Sensor {
    public:
        /**
         * Constructor 
         * 
         * @param *spi bus to use for this Thermocouple
         * @param csPin chip select pin to use for this Thermocouple
         * @param updateInterval in ms to poll for new data
         **/
        SensorThermo(SPIClass *spi, uint8_t csPin, uint16_t updateInterval);

        /**
         * Begin the Thermocouple sensor by setting up over SPI
         **/
        void begin();

        /**
         * Polls Thermocouple for any new data based on the specified interval
         **/
        void handle();

        String getHumanName();

        /**
         * @return current temperature in Celsius
         **/
        double getTemp();

    private:
        Adafruit_MAX31855* _probe;
        SPIClass *_spi;
        uint16_t _updateInterval;
        uint8_t _csPin;
        unsigned long int _lastUpdate;
        double _temp;
};

#endif
