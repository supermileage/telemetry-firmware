#include "SensorThermo.h"

/**
 * Constructor 
 * @param *spi bus to use for this Thermocouple
 * @param csPin chip select pin to use for this Thermocouple
 * @param updateInterval in ms to poll for new data
 **/
SensorThermo::SensorThermo(SPIClass *spi, uint8_t csPin, uint16_t updateInterval){
    _spi = spi;
    _probe = new Adafruit_MAX31855(csPin,spi);
    _updateInterval = updateInterval;
    _csPin = csPin;
}

String SensorThermo::getHumanName() {
    return "Thermo_" + String(_csPin);
}

/**
 * Begin the Thermocouple sensor by setting up over SPI
 **/
void SensorThermo::begin() {
    _spi->begin();
    _probe->begin();
    _lastUpdate = millis();
}

/**
 * Polls Thermocouple for any new data based on the specified interval
 **/
void SensorThermo::handle() {
    if(millis() > _lastUpdate + _updateInterval) {
        _lastUpdate = millis();
        _temp = _probe->readCelsius();
    }
}

/**
 * @return current temperature in Celsius
 **/
double SensorThermo::getTemp() {
    return _temp;
}