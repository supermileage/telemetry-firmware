#include "CanSensorBms.h"
#include "settings.h"

CanSensorBms::CanSensorBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanListener(canInterface, CAN_BMS_RESPONSE), _requestIntervalMs(requestIntervalMs) {}

void CanSensorBms::handle() {
    if(millis() - _lastValidTime >= _requestIntervalMs) {

        CanMessage msg = CAN_MESSAGE_NULL;
        msg.id = CAN_BMS_REQUEST;
        msg.dataLength = REQ_DATA_LENGTH;
        msg.data[0] = _paramIds[_currentParam];
            
        _canInterface.sendMessage(msg);

        if(_currentParam == NUM_PARAMS) {
            _currentParam = 0;
        } else {
            _currentParam++;
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
        switch (message.data[RSP_PARAM_ID_BYTE]) {
            case PARAM_ID_BATTERY_VOLTAGE:
                _batteryVoltage = parseFloat(message.data);
                break;
            case PARAM_ID_BATTERY_CURRENT:
                _batteryCurrent = parseFloat(message.data);
                break;
            case PARAM_ID_MAX_CELL_VOLTAGE:
                _cellVoltageMax = (float)parseInt16(message.data) / 1000.0; 
                break;
            case PARAM_ID_MIN_CELL_VOLTAGE:
                _cellVoltageMin = (float)parseInt16(message.data) / 1000.0; 
                break;
            case PARAM_ID_STATUS: {
                unsigned statusCode = parseInt16(message.data);
                if(statusCode == STATUS_CHARGING) {
                    _bmsStatus = "charge";
                }
                else if(statusCode == STATUS_CHARGED) {
                    _bmsStatus = "charged";
                }
                else if(statusCode == STATUS_DISCHARGING) {
                    _bmsStatus = "discharge";
                }
                else if(statusCode == STATUS_REGENERATION) {
                    _bmsStatus = "regen";
                }
                else if(statusCode == STATUS_IDLE) {
                    _bmsStatus = "idle";
                }
                else {
                    _bmsStatus = "fault";
                }
                break;
            }
            case PARAM_ID_SOC:
                _soc = (float)parseInt32(message.data) / 1000000.0;    
                break;
            case PARAM_ID_TEMP:
                if(message.data[5] == TEMP_ID_INTERNAL) {
                    _tempBms = parseInt16(message.data + 1) / 10;
                }
                else if(message.data[5] == TEMP_ID_BATTERY_1) {
                    _batteryTemp1 = parseInt16(message.data + 1) / 10;
                }
                else if(message.data[5] == TEMP_ID_BATTERY_2) {
                    _batteryTemp2 = parseInt16(message.data + 1) / 10;
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
    memcpy((void*)&output, (void*)(dataPtr + RSP_DATA_BYTE), 4);
    return output;
}

uint16_t CanSensorBms::parseInt16(uint8_t* dataPtr) {
    return ((uint16_t)dataPtr[RSP_DATA_BYTE + 1] << 8) 
            | dataPtr[RSP_DATA_BYTE];
}

uint32_t CanSensorBms::parseInt32(uint8_t* dataPtr) {
    return ((uint32_t)dataPtr[RSP_DATA_BYTE + 3] << 24) 
            | ((uint32_t)dataPtr[RSP_DATA_BYTE + 2] << 16)
            | ((uint32_t)dataPtr[RSP_DATA_BYTE + 1] << 8)
            | dataPtr[RSP_DATA_BYTE];
}