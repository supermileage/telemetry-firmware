#include "SensorEcu.h"

SensorEcu::SensorEcu(USARTSerial *serial)
{
    _serial = serial;
}

String SensorEcu::getHumanName() {
    return "ECU";
}

void SensorEcu::begin()
{
    _serial->begin(115200, SERIAL_8N1);
}

void SensorEcu::flush()
{
    while (_serial->available())
    {
        _serial->read();
    }
}

void SensorEcu::handle()
{
    if (_serial->available() < 27)
    {
        return;
    }
    uint8_t buffer[27];

    _serial->readBytes((char *)buffer, 27);

    uint8_t header1 = buffer[0] & 0xFF;
    uint8_t header2 = buffer[1] & 0xFF;
    uint8_t header3 = buffer[2] & 0xFF;

    uint8_t dataLengthField = buffer[3] & 0xFF;
    uint8_t serviceId = buffer[4] & 0xFF;

    int checkSum = 0;
    for(int i = 0; i < 26; i++) {
        checkSum += buffer[i];
    }

    if (
        header1 == 0x80 &&
        header2 == 0x8F &&
        header3 == 0xEA &&
        dataLengthField == 0x16 &&
        serviceId == 0x50 &&
        checkSum == buffer[26]){
        // Data is valid
        _rpm = this->_interpretValue(buffer[6], buffer[7], 0.25, 0.0);
        _map = this->_interpretValue(buffer[8], buffer[9], 0.0039, 0.0);
        _tps = this->_interpretValue(buffer[10], buffer[11], 0.0015, 0.0);
        _ect = this->_interpretValue(buffer[12], buffer[13], 1, -40.0);
        _iat = this->_interpretValue(buffer[14], buffer[15], 1, -40.0);
        _o2s = this->_interpretValue(buffer[16], buffer[17], 0.0048, 0.0);
        _spark = this->_interpretValue(buffer[18], buffer[19], 0.5, 0.0);
        _fuelPW1 = this->_interpretValue(buffer[20], buffer[21], 0.001, 0.0);
        _fuelPW2 = this->_interpretValue(buffer[22], buffer[23], 0.001, 0.0);
        _ubAdc = this->_interpretValue(buffer[24], buffer[25], 0.00625, 0.0);
    }else{
        flush();
    }
}

String SensorEcu::getRPM()
{
    return String::format("%.0f",this->_rpm);
}

String SensorEcu::getMap()
{
    return String::format("%.2f",this->_map);
}

String SensorEcu::getTPS()
{
    return String::format("%.0f",this->_tps);
}

String SensorEcu::getECT()
{
    return String::format("%.0f",this->_ect);
}

String SensorEcu::getIAT()
{
    return String::format("%.0f",this->_iat);
}

String SensorEcu::getO2S()
{
    return String::format("%.2f",this->_o2s);
}

String SensorEcu::getSpark()
{
    return String::format("%.0f",this->_spark);
}

String SensorEcu::getFuelPW1()
{
    return String::format("%.3f",this->_fuelPW1);
}

String SensorEcu::getFuelPW2()
{
    return String::format("%.3f",this->_fuelPW2);
}

String SensorEcu::getUbAdc()
{
    return String::format("%.2f",this->_ubAdc);
}

float SensorEcu::_interpretValue(uint8_t high, uint8_t low, float factor, float offset)
{
    return (float)((int)high * 256 + (int)low) * factor + offset;
}

