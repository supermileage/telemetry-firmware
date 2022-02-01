#include "CanSensorBms.h"
#include "settings.h"

CanSensorBms::CanSensorBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanListener(canInterface, CAN_BMS_RESPONSE), _requestIntervalMs(requestIntervalMs) {}

void CanSensorBms::handle() {
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

String CanSensorBms::getHumanName() {
    return "CanSensorBms";
}

void CanSensorBms::update(CanMessage message) {

    if(message.data[RSP_STATUS_BYTE] != TRUE) {
        DEBUG_SERIAL_LN("Poor BMS Data Received");
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
                }
                else if(statusCode == STATUS_CHARGED) {
                    _bmsStatus = "BMS Charged!";
                }
                else if(statusCode == STATUS_DISCHARGING) {
                    _bmsStatus = "BMS Discharging...";
                }
                else if(statusCode == STATUS_REGENERATION) {
                    _bmsStatus = "BMS Regeneration";
                }
                else if(statusCode == STATUS_IDLE) {
                    _bmsStatus = "BMS Idle";
                }
                else {
                    _bmsStatus = "BMS Fault Error";
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
                else if(message.data[5] == TEMP_ID_BATTERY_2) {
                    _batteryTemp2 = (message.data[4] << 8) + message.data[3];
                }
                break;
            default:
                break;
        }
    }
}

String CanSensorBms::getBatteryVolt() {
    return FLOAT_TO_STRING(_batteryVoltage, 1);
}

String CanSensorBms::getBatteryCurrent() {
    return FLOAT_TO_STRING(_batteryCurrent, 3);
}

String CanSensorBms::getMaxVolt() {
    return FLOAT_TO_STRING(_cellVoltageMax, 2);
}

String CanSensorBms::getMinVolt() {
    return FLOAT_TO_STRING(_cellVoltageMin, 2);
}

String CanSensorBms::getSoc() {
    return FLOAT_TO_STRING(_soc, 1); 
}

String CanSensorBms::getStatusBms() {
    return _bmsStatus;
}

int CanSensorBms::getTempBms() {
    return _tempBms;
}

int CanSensorBms::getBatteryTemp1() {
    return _batteryTemp1;
}

int CanSensorBms::getBatteryTemp2() {
    return _batteryTemp2;
}

float CanSensorBms::parseFloat(uint8_t* dataPtr) {
    float output;
    memcpy((void*)&output, (void*)dataPtr, 4);
    return output;
}