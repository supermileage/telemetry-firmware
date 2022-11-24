#include "SensorThermo.h"
#include "settings.h"

// #define DEBUG_THERMO


#define DEBUG_THERMO_INTERVAL 250

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
    #ifdef DEBUG_THERMO
        if(millis() > _lastDebug + DEBUG_THERMO_INTERVAL) {
            _lastDebug = millis();

            DEBUG_SERIAL(getHumanName());

            uint8_t error = _probe->readError();
            switch(error) {
                case 1: 
                    DEBUG_SERIAL_LN(" ERROR: No Thermocouple Detected");
                    break;
                case 2:
                    DEBUG_SERIAL_LN(" ERROR: Thermocouple Shorted To GND");
                    break;
                case 4:
                    DEBUG_SERIAL_LN(" ERROR: Thermocouple Shorted To VCC");
                    break;
                default:
                    DEBUG_SERIAL_LN(" " + FLOAT_TO_STRING(_probe->readCelsius(),2) + "°C");
                    break;
            }
        
        }
    #endif
}

int SensorThermo::getProbeTemp(bool &valid) {
    double value = _probe->readCelsius();
    valid = !_probe->readError();
    return value;
}

int SensorThermo::getInternalTemp(bool &valid) {
    valid = true;
    return _probe->readInternal();
}