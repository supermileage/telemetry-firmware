#include "SensorEcu.h"
#include "settings.h"

#include <cmath>

#define ECU_BAUD                115200

const int32_t SensorEcu::PacketSize = 27;
const uint8_t SensorEcu::Header1 = 0x80;
const uint8_t SensorEcu::Header2 = 0x8F;
const uint8_t SensorEcu::Header3 = 0xEA;
const uint8_t SensorEcu::DataFieldLength = 0x16;
const uint8_t SensorEcu::ServiceId = 0x50;

SensorEcu::SensorEcu(TelemetrySerial *serial) : _serial(serial) { }

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
    
    if (_serial->available() < SensorEcu::PacketSize) {
        return;
    }

    uint8_t buffer[27] = { };

    _serial->readBytes((char*)buffer, SensorEcu::PacketSize);

    // Check if the header is correct
    if (buffer[0] == SensorEcu::Header1 && 
        buffer[1] == SensorEcu::Header2 && 
        buffer[2] == SensorEcu::Header3 && 
        buffer[3] == SensorEcu::DataFieldLength && 
        buffer[4] == SensorEcu::ServiceId) {

			uint8_t checkSum = 0;
			for(int i = 0; i < 26; i++) {
				checkSum += buffer[i];
			}

            // Check if the checksum is correct
            if(checkSum == buffer[26]){

                _lastUpdate = millis();
				_valid = true;

                // Data is valid, update all fields
                _rpm = _interpretValue(buffer[6], buffer[7], 0.25, 0.0, true);
                _map = _interpretValue(buffer[8], buffer[9], 0.0039, 0.0);
                _tps = _interpretValue(buffer[10], buffer[11], 0.0015, 0.0, true);
                _ect = _interpretValue(buffer[12], buffer[13], 1, -40.0, true);
                _iat = _interpretValue(buffer[14], buffer[15], 1, -40.0, true);
                _o2s = _interpretValue(buffer[16], buffer[17], 0.0048, 0.0);
                _spark = _interpretValue(buffer[18], buffer[19], 0.5, 0.0, true);
                _fuelPW1 = _interpretValue(buffer[20], buffer[21], 0.001, 0.0);
                _fuelPW2 = _interpretValue(buffer[22], buffer[23], 0.001, 0.0);
                _ubAdc = _interpretValue(buffer[24], buffer[25], 0.00625, 0.0);

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
    return _rpm;
}

String SensorEcu::getMap(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_map, 2);
}

int SensorEcu::getTPS(bool &valid) {
    valid = _valid;
    return _tps;
}

int SensorEcu::getECT(bool &valid) {
    valid = _valid;
    return _ect;
}

int SensorEcu::getIAT(bool &valid) {
    valid = _valid;
    return _iat;
}

String SensorEcu::getO2S(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_o2s, 2);
}

int SensorEcu::getSpark(bool &valid) {
    valid = _valid;
    return _spark;
}

String SensorEcu::getFuelPW1(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_fuelPW1, 3);
}

String SensorEcu::getFuelPW2(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_fuelPW2, 3);
}

String SensorEcu::getUbAdc(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_ubAdc, 1);
}

int SensorEcu::getOn(bool &valid) {
    valid = true;
    return _valid;
}

float SensorEcu::_interpretValue(uint8_t high, uint8_t low, float factor, float offset, bool isInt) {
	if (isInt)
		return round((((uint32_t)high << 8) + (uint32_t)low) * factor + offset);
	else
		return (((uint32_t)high << 8) + (uint32_t)low) * factor + offset;
}

