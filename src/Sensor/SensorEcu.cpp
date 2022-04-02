#include "SensorEcu.h"
#include "settings.h"

#define ECU_BAUD                115200

#define ECU_PACKET_SIZE         27

#define ECU_HEADER_1            0x80
#define ECU_HEADER_2            0x8F
#define ECU_HEADER_3            0xEA
#define ECU_DATA_FIELD_LENGTH   0x16
#define ECU_SERVICE_ID          0x50

SensorEcu::SensorEcu(USARTSerial *serial) {
    _serial = serial;
}

String SensorEcu::getHumanName() {
    return "ECU";
}

void SensorEcu::begin() {
    _serial->begin(ECU_BAUD, SERIAL_8N1);
}

void SensorEcu::flush() {
    while (_serial->available())
    {
        _serial->read();
    }
}

void SensorEcu::handle() {

    if(millis() < _lastUpdate + STALE_INTERVAL) {
        _valid = true;
    } else {
        _valid = false;
    }
    
    if (_serial->available() < ECU_PACKET_SIZE) {
        return;
    }

    uint8_t buffer[27];

    _serial->readBytes((char *)buffer, ECU_PACKET_SIZE);

    uint8_t checkSum = 0;
    for(int i = 0; i < 26; i++) {
        checkSum += buffer[i];
    }

    // Check if the header is correct
    if (buffer[0] == ECU_HEADER_1 && 
        buffer[1] == ECU_HEADER_2 && 
        buffer[2] == ECU_HEADER_3 && 
        buffer[3] == ECU_DATA_FIELD_LENGTH && 
        buffer[4] == ECU_SERVICE_ID) {

            // Check if the checksum is correct
            if(checkSum == buffer[26]){

                _lastUpdate = millis();

                // Data is valid, update all fields
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

            } else {
                DEBUG_SERIAL_LN("ERROR: ECU Packet Checksum Invalid");
            }

    }else{
        // The header is not correct, flush the serial buffer
        flush();
    }

}

int SensorEcu::getRPM(bool &valid) {
    valid = _valid;
    return this->_rpm;
}

String SensorEcu::getMap(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(this->_map, 2);
}

int SensorEcu::getTPS(bool &valid) {
    valid = _valid;
    return this->_tps;
}

int SensorEcu::getECT(bool &valid) {
    valid = _valid;
    return this->_ect;
}

int SensorEcu::getIAT(bool &valid) {
    valid = _valid;
    return this->_iat;
}

String SensorEcu::getO2S(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(this->_o2s, 2);
}

int SensorEcu::getSpark(bool &valid) {
    valid = _valid;
    return this->_spark;
}

String SensorEcu::getFuelPW1(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(this->_fuelPW1, 3);
}

String SensorEcu::getFuelPW2(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(this->_fuelPW2, 3);
}

String SensorEcu::getUbAdc(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(this->_ubAdc, 1);
}

float SensorEcu::_interpretValue(uint8_t high, uint8_t low, float factor, float offset) {
    return (float)((int)high * 256 + (int)low) * factor + offset;
}

