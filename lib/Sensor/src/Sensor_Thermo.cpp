#include "Sensor_Thermo.h"

SparkFunMAX31855k probe(10);

Sensor_Thermo::Sensor_Thermo(uint8_t csPin){
    SPI.begin();
    probe.setCS(csPin);
}