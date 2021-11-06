#include "SensorVoltage.h"

#define INPUT_VOLTAGE_PIN A1
#define ANALOG_CONVERT_RAW_TO_3V 1241
#define R1 10000
#define R2 2200

SensorVoltage::SensorVoltage() {
    pinMode(INPUT_VOLTAGE_PIN, INPUT);
}

String SensorVoltage::getHumanName() {
    return "Boron Input Voltage";
}

void SensorVoltage:: begin() {
}

void SensorVoltage::handle() {
}

float SensorVoltage::getVoltage() {
    float in3v3 = (analogRead(INPUT_VOLTAGE_PIN))/ (float) ANALOG_CONVERT_RAW_TO_3V;
    return ((in3v3*(R1+R2))/R2);
}
