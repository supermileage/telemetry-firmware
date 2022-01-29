#include "SensorBms.h"

#define TEMP_ID_INTERNAL    0x00
#define TEMP_ID_BATTERY_1   0x01
#define TEMP_ID_BATTERY_2   0x02

#define REQ_DATA_LENGTH     8

#define RSP_STATUS_BYTE     0x0
#define RSP_PARAM_ID_BYTE   0x1
#define RSP_DATA_BYTE       0x2

SensorBms::SensorBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanListener(canInterface, CAN_BMS_RESPONSE), _requestIntervalMs(requestIntervalMs) {}

void SensorBms::handle() {
    if(millis() - _lastValidTime >= _requestIntervalMs) {

        for(int i = 0; i < NUM_PARAMS; i++) {
            CanMessage msg = CAN_MESSAGE_NULL;
            msg.id = CAN_BMS_REQUEST;
            msg.dataLength = REQ_DATA_LENGTH;
            msg.data[0] = _paramIds[i];
            
            _canInterface.sendMessage(msg);
        }
        
        _lastValidTime = millis();
    }
}

String SensorBms::getHumanName() {
    return "BMS";
}

void SensorBms::update(CanMessage message) {
    DEBUG_SERIAL_LN("RECEIVED RESPONSE FROM BMS");

    if(message.data[RSP_STATUS_BYTE] != TRUE) {
        DEBUG_SERIAL_LN("Poor Data Received");
    }
    else {
        unsigned statusCode = 0;
        switch (message.data[RSP_PARAM_ID_BYTE]) {
            case PARAM_ID_BATTERY_VOLTAGE:
                _batteryVoltage = parseFloat(message.data + RSP_DATA_BYTE);
                break;
            case PARAM_ID_BATTERY_CURRENT:
                _batteryCurrent = parseFloat(message.data + RSP_DATA_BYTE);
                break;
            case PARAM_ID_MAX_CELL_VOLTAGE:
                _cellVoltageMax = parseFloat(message.data + RSP_DATA_BYTE);    
                break;
            case PARAM_ID_MIN_CELL_VOLTAGE:
                _cellVoltageMin = parseFloat(message.data + RSP_DATA_BYTE);    
                break;
            case PARAM_ID_STATUS:
                statusCode = (message.data[3] << 8) + message.data[2];
                if(statusCode == STATUS_CHARGING) {
                    _bmsStatus = "BMS Charging...";
                    DEBUG_SERIAL_LN("BMS Charging...");
                }
                else if(statusCode == STATUS_CHARGED) {
                    _bmsStatus = "BMS Charged!";
                    DEBUG_SERIAL_LN("BMS Charged!");
                }
                else if(statusCode == STATUS_DISCHARGING) {
                    _bmsStatus = "BMS Discharging...";
                    DEBUG_SERIAL_LN("BMS Discharging...");
                }
                else if(statusCode == STATUS_REGENERATION) {
                    _bmsStatus = "BMS Regeneration";
                    DEBUG_SERIAL_LN("BMS Regeneration");
                }
                else if(statusCode == STATUS_IDLE) {
                    _bmsStatus = "BMS Idle";
                    DEBUG_SERIAL_LN("BMS Idle");
                }
                else {
                    _bmsStatus = "BMS Fault Error";
                    DEBUG_SERIAL_LN("BMS Fault Error");
                }
                break;
            case PARAM_ID_SOC:
                _soc = parseFloat(message.data + RSP_DATA_BYTE);    
                break;
            case PARAM_ID_TEMP:
                if(message.data[5] == TEMP_ID_INTERNAL) {
                    _tempBms = (message.data[4] << 8) + message.data[3];
                }
                else if(message.data[5] == TEMP_ID_BATTERY_1) {
                    _batteryTemp1 = (message.data[4] << 8) + message.data[3];
                }
                else {
                    _batteryTemp2 = (message.data[4] << 8) + message.data[3];
                }
                break;
            default:
                break;
        }
    }
}

float SensorBms::getBatteryVolt() {
    return _batteryVoltage;
}

float SensorBms::getBatteryCurrent() {
    return _batteryCurrent;
}

float SensorBms::getMaxVolt() {
    return _cellVoltageMax;
}

float SensorBms::getMinVolt() {
    return _cellVoltageMin;
}

float SensorBms::getSoc() {
    return _soc; 
}

String SensorBms::getStatusBms() {
    return _bmsStatus;
}

int SensorBms::getTempBms() {
    return _tempBms;
}

int SensorBms::getBatteryTemp1() {
    return _batteryTemp1;
}

int SensorBms::getBatteryTemp2() {
    return _batteryTemp2;
}

float SensorBms::parseFloat(uint8_t* dataPtr) {
    float output;
    memcpy((void*)&output, (void*)dataPtr, 4);
    return output;
}