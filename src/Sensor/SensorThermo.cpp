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

            Serial.print(getHumanName());

            uint8_t error = _probe->readError();
            switch(error) {
                case 1: 
                    Serial.println(" ERROR: No Thermocouple Detected");
                    break;
                case 2:
                    Serial.println(" ERROR: Thermocouple Shorted To GND");
                    break;
                case 4:
                    Serial.println(" ERROR: Thermocouple Shorted To VCC");
                    break;
                default:
                    Serial.println(" " + FLOAT_TO_STRING(_probe->readCelsius(),2) + "°C");
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