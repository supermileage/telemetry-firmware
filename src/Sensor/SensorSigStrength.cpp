#include "SensorSigStrength.h"

SensorSigStrength::SensorSigStrength() {
}

String SensorSigStrength::getHumanName() {
    return "Boron Signal Strength and Quality";
}

void SensorSigStrength:: begin() {
}

void SensorSigStrength::handle() {
}

int SensorSigStrength::getStrength() {
    _sig = Cellular.RSSI();
    int value = _sig.getStrength();
    if(value < 0) value = 0;
    return value;
}

int SensorSigStrength:: getQuality() {
    _sig = Cellular.RSSI();
    int value = _sig.getQuality();
    if(value < 0) value = 0;
    return value;
}