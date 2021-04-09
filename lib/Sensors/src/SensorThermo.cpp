#include "SensorThermo.h"

SensorThermo::SensorThermo(SPIClass *spi, uint8_t csPin, uint16_t updateInterval){
    _spi = spi;
    _probe = new Adafruit_MAX31855(csPin,spi);
    _updateInterval = updateInterval;
}

void SensorThermo::begin() {
    _spi->begin();
    _probe->begin();
    _lastUpdate = millis();
}

void SensorThermo::handle() {
    if(millis() > _lastUpdate + _updateInterval) {
        _lastUpdate = millis();
        long unsigned int start = micros();
        _temp = _probe->readCelsius();
        Serial.println("Thermo read: " + String(micros() - start) + "us");
    }
}

double SensorThermo::getTemp() {
    return _temp;
}