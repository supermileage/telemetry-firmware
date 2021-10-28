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
    return (int) _sig.getStrength();
}

float SensorSigStrength:: getQuality() {
    _sig = Cellular.RSSI();
    return (int) _sig.getQuality();
}