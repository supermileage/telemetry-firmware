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

int SensorThermo::getProbeTemp(bool &valid) {
    int value = _probe->readCelsius();
    valid = (value != NAN);
    return value;
}

int SensorThermo::getInternalTemp(bool &valid) {
    int value = _probe->readInternal();
    valid = (value != NAN);
    return value;
}