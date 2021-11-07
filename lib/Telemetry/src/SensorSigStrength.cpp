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

String SensorSigStrength::getStrength() {
    _sig = Cellular.RSSI();
    return String::format("%.0f",_sig.getStrength());
}

String SensorSigStrength:: getQuality() {
    _sig = Cellular.RSSI();
    return String::format("%.0f",_sig.getQuality());
}