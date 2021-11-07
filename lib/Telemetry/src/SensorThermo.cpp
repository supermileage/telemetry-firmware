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
    return String::format("%.0f",_probe->readCelsius());
}

String SensorThermo::getInternalTemp() {
    return String::format("%.0f",_probe->readInternal());
}