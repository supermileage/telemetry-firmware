#ifndef _SENSOR_THERMO_H_
#define _SENSOR_THERMO_H_

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
        SensorThermo(SPIClass *spi, uint8_t csPin);

        /**
         * Begin the Thermocouple sensor by setting up over SPI
         **/
        void begin() override;

        /**
         * Polls Thermocouple for any new data based on the specified interval
         **/
        void handle() override;

        String getHumanName() override;

        /**
         * @return current probe temperature in Celsius
         **/
        int getProbeTemp(bool &valid = Sensor::dummy);

        /**
         * @return current internal temperature in Celsius
         **/
        int getInternalTemp(bool &valid = Sensor::dummy);

        String getInitStatus();

    private:
        Adafruit_MAX31855* _probe;
        SPIClass *_spi;
        uint8_t _csPin;
        uint32_t _lastDebug = 0;
        bool _initialized = false;
};

#endif
