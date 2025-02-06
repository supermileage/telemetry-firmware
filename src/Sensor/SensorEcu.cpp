#include "SensorEcu.h"
#include "settings.h"

#include <cmath>

// #define DEBUG_SENSOR_ECU

#ifdef DEBUG_SENSOR_ECU
#define DEBUG_INTERVAL 100
uint32_t sensor_ecu_last_debug_output = 0;
#endif

#define SENSOR_ECU_PACKET_SIZE 27

const int32_t SensorEcu::PacketSize = SENSOR_ECU_PACKET_SIZE;
const unsigned SensorEcu::Baud = 115200;
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
    _serial->begin(SensorEcu::Baud, SERIAL_8N1);
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

    if (millis() >= _lastUpdate + ECU_ON_OFF_INTERVAL) {
        _isOn = false;
    }
	
	int bytesAvail = _serial->available();
    if (bytesAvail < SensorEcu::PacketSize) {
		#ifdef DEBUG_SENSOR_ECU
		if (bytesAvail > 0 && millis() >= sensor_ecu_last_debug_output + DEBUG_INTERVAL) {
			sensor_ecu_last_debug_output = millis();
			DEBUG_SERIAL_F("Received %d bytes from ECU\n", _serial->available());
		}
		#endif
        return;
    }

    uint8_t buffer[SENSOR_ECU_PACKET_SIZE] = { };

    _serial->readBytes((char*)buffer, SensorEcu::PacketSize);

	#ifdef DEBUG_SENSOR_ECU
		DEBUG_SERIAL_LN("-----------------------------");
		DEBUG_SERIAL("SensorEcu Received Message - Header: ");
		DEBUG_SERIAL_F("0x%x 0x%x 0x%x\n", buffer[0], buffer[1], buffer[2]);
		DEBUG_SERIAL_F("Data Field Length: 0x%x\n", buffer[3]);
		DEBUG_SERIAL_F("Service Id: 0x%x\n", buffer[4]);

		for (int i = 5; i < SensorEcu::PacketSize - 1; i++) { // print the data
			DEBUG_SERIAL_F("0x%x\t", buffer[i]);
		}
		DEBUG_SERIAL_LN();
	#endif

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
                _isOn = true;

            } else {
                DEBUG_SERIAL_LN("ERROR: ECU Packet Checksum Invalid");
            }

    }else{
        // The header is not correct, flush the serial buffer
		DEBUG_SERIAL_LN("ERROR: ECU Packet header incorrect");
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
    return _isOn;
}

float SensorEcu::_interpretValue(uint8_t high, uint8_t low, float factor, float offset, bool isInt) {
	if (isInt)
		return round(((high << 8) | low) * factor + offset);
	else
		return ((high << 8) | low) * factor + offset;
}

