#include "SensorThermo.h"

SensorThermo::SensorThermo(SPIClass *spi, uint8_t csPin){
    _spi = spi;
    _probe = new Adafruit_MAX31855(csPin,spi);
    _csPin = csPin;
}

String SensorThermo::getHumanName() {
    return "Thermo_" + String(_csPin);
}

void SensorThermo::begin() {
    _spi->begin();
    _probe->begin();
}

void SensorThermo::handle() {
}

String SensorThermo::getProbeTemp() {
    int value = _probe->readCelsius();
    if (value == NAN) value = 0;
    return String(value);
}

String SensorThermo::getInternalTemp() {
    int value = _probe->readInternal();
    if (value == NAN) value = 0;
    return String(value);
}