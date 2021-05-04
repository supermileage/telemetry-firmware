#include "SensorCan.h"

/**
 * Constructor 
 **/
SensorCan::SensorCan(SPIClass *spi, uint8_t csPin, uint8_t intPin) {
    _spi = spi;
    _csPin = csPin;
    _intPin = intPin;
}

String SensorCan::getHumanName() {
    return "CAN";
}

/**
 * 
 **/
void SensorCan::begin() {

}

/**
 *
 **/
void SensorCan::handle() {

}