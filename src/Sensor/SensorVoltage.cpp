#include "SensorVoltage.h"

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

String SensorVoltage::getVoltage(bool &valid) {
    valid = true;
    
    float in3v3 = (analogRead(INPUT_VOLTAGE_PIN))/ (float) ANALOG_CONVERT_RAW_TO_3V;
    return String::format("%.1f",(in3v3*(R1+R2))/R2);
}
