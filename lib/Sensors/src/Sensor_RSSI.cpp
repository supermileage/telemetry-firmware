#include "Sensor_RSSI.h"

Sensor_RSSI::Sensor_RSSI(){
}

int Sensor_RSSI::get(){
    sig = Cellular.RSSI();
    return round(sig.getStrength());
}