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

float SensorSigStrength::getStrength() {
    _sig = Cellular.RSSI();
    _strength = (int) _sig.getStrength();
    return _strength;
}

float SensorSigStrength:: getQuality() {
    _sig = Cellular.RSSI();
    _quality = (int) _sig.getQuality();
    return _quality;
}