#include "SensorThermo.h"

SensorThermo::SensorThermo(SPIClass *spi, uint8_t csPin, uint16_t updateInterval){
    _spi = spi;
    _probe = new Adafruit_MAX31855(csPin,spi);
    _updateInterval = updateInterval;
    _csPin = csPin;
}

String SensorThermo::getHumanName() {
    return "Thermo_" + String(_csPin);
}

void SensorThermo::begin() {
    _spi->begin();
    _probe->begin();
    _lastUpdate = millis();
}

void SensorThermo::handle() {
    if(millis() > _lastUpdate + _updateInterval) {
        _lastUpdate = millis();
        _temp = _probe->readCelsius();
    }
}

double SensorThermo::getTemp() {
    return _temp;
}