#include "CanSensorTinyBms.h"
#include "settings.h"

#define PARAM_ID_BATTERY_VOLTAGE    0x14
#define PARAM_ID_BATTERY_CURRENT    0x15
#define PARAM_ID_MAX_CELL_VOLTAGE   0x16
#define PARAM_ID_MIN_CELL_VOLTAGE   0x17
#define PARAM_ID_STATUS             0x18
#define PARAM_ID_SOC                0x1A
#define PARAM_ID_TEMP               0x1B

#define STATUS_CHARGING             0x91
#define STATUS_CHARGED              0x92
#define STATUS_DISCHARGING          0x93
#define STATUS_REGENERATION         0x96
#define STATUS_IDLE                 0x97
#define STATUS_FAULT_ERROR          0x9B

#define TEMP_ID_INTERNAL    0x00
#define TEMP_ID_BATTERY_1   0x01
#define TEMP_ID_BATTERY_2   0x02

const uint8_t VALIDATION_IDS[] {
    PARAM_ID_BATTERY_VOLTAGE,
    PARAM_ID_BATTERY_CURRENT,
    PARAM_ID_MAX_CELL_VOLTAGE,
    PARAM_ID_MIN_CELL_VOLTAGE,
    PARAM_ID_STATUS,
    PARAM_ID_SOC,
    PARAM_ID_TEMP,
    TEMP_ID_INTERNAL,
    TEMP_ID_BATTERY_1,
    TEMP_ID_BATTERY_2
};

CanSensorTinyBms::CanSensorTinyBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanListener(canInterface, CAN_TINYBMS_RESPONSE), _requestIntervalMs(requestIntervalMs) {
        // create dictionary of validation properties with 
        for (auto id : VALIDATION_IDS)  {
            _validationMap[id] = 0;
        }
    }

void CanSensorTinyBms::handle() {
    if(millis() - _lastValidTime >= _requestIntervalMs) {

        CanMessage msg = CAN_MESSAGE_NULL;
        msg.id = CAN_TINYBMS_REQUEST;
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

String CanSensorTinyBms::getHumanName() {
    return "CanSensorTinyBms";
}

void CanSensorTinyBms::update(CanMessage message) {

    if(message.data[RSP_STATUS_BYTE] != TRUE) {
        DEBUG_SERIAL_LN("Poor BMS Data Received");
    }
    else {
        uint8_t id = message.data[RSP_PARAM_ID_BYTE];
        uint64_t time = millis();
        switch (id) {
            case PARAM_ID_BATTERY_VOLTAGE:
                _batteryVoltage = parseFloat(message.data);
                break;
            case PARAM_ID_BATTERY_CURRENT:
                // tiny bms returns -A for discharging and +A for charging: we want the inverse so flip sign here
                _batteryCurrent = -parseFloat(message.data);
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
                    _bmsStatus = Charging;
                }
                else if(statusCode == STATUS_CHARGED) {
                    _bmsStatus = Charged;
                }
                else if(statusCode == STATUS_DISCHARGING) {
                    _bmsStatus = Discharging;
                }
                else if(statusCode == STATUS_REGENERATION) {
                    _bmsStatus = Regeneration;
                }
                else if(statusCode == STATUS_IDLE) {
                    _bmsStatus = Idle;
                }
                else
                {
                    _bmsStatus = FaultError;
                }
                break;
            }
            case PARAM_ID_SOC:
                _soc = (float)parseInt32(message.data) / 1000000.0;
                break;
            case PARAM_ID_TEMP:
                if(message.data[5] == TEMP_ID_INTERNAL) {
                    _tempBms = parseInt16(message.data + 1) / 10;
                    _validationMap[TEMP_ID_INTERNAL] = time;
                }
                else if(message.data[5] == TEMP_ID_BATTERY_1) {
                    _batteryTemp1 = parseInt16(message.data + 1) / 10;
                    _validationMap[TEMP_ID_BATTERY_1] = time;
                }
                else if(message.data[5] == TEMP_ID_BATTERY_2) {
                    _batteryTemp2 = parseInt16(message.data + 1) / 10;
                    _validationMap[TEMP_ID_BATTERY_2] = time;
                }
                break;
            default:
                break;
        }

		// update validation map--exclude temp because it needs special handling for 3 cases
		if (id != PARAM_ID_TEMP && _validationMap.find(id) != _validationMap.end()) {
			_validationMap[id] = time;
		}
    }
}

String CanSensorTinyBms::getBatteryVolt(bool& valid) {
    valid  = _validate(PARAM_ID_BATTERY_VOLTAGE);
    return FLOAT_TO_STRING(_batteryVoltage, 1);
}

String CanSensorTinyBms::getBatteryCurrent(bool& valid) {
    valid  = _validate(PARAM_ID_BATTERY_CURRENT);
    return FLOAT_TO_STRING(_batteryCurrent, 1);
}

String CanSensorTinyBms::getMaxVolt(bool& valid) {
    valid  = _validate(PARAM_ID_MAX_CELL_VOLTAGE);
    return FLOAT_TO_STRING(_cellVoltageMax, 2);
}

String CanSensorTinyBms::getMinVolt(bool& valid) {
    valid  = _validate(PARAM_ID_MIN_CELL_VOLTAGE);
    return FLOAT_TO_STRING(_cellVoltageMin, 2);
}

String CanSensorTinyBms::getSoc(bool& valid) {
    valid  = _validate(PARAM_ID_SOC);
    return FLOAT_TO_STRING(_soc, 1); 
}

int CanSensorTinyBms::getStatusBms(bool& valid) {
    valid  = _validate(PARAM_ID_STATUS);
    return _bmsStatus;
}

String CanSensorTinyBms::getStatusBmsString(bool& valid) {
    valid  = _validate(PARAM_ID_STATUS);
    return String(bmsStatuses[_bmsStatus]);
}

int CanSensorTinyBms::getTempBms(bool& valid) {
    valid  = _validate(TEMP_ID_INTERNAL);
    return _tempBms;
}

int CanSensorTinyBms::getBatteryTemp1(bool& valid) {
    valid  = _validate(TEMP_ID_BATTERY_1);
    return _batteryTemp1;
}

int CanSensorTinyBms::getBatteryTemp2(bool& valid) {
    valid  = _validate(TEMP_ID_BATTERY_2);
    return _batteryTemp2;
}

float CanSensorTinyBms::parseFloat(uint8_t* dataPtr) {
    float output;
    memcpy((void*)&output, (void*)(dataPtr + RSP_DATA_BYTE), 4);
    return output;
}

uint16_t CanSensorTinyBms::parseInt16(uint8_t* dataPtr) {
    return ((uint16_t)dataPtr[RSP_DATA_BYTE + 1] << 8)
            | dataPtr[RSP_DATA_BYTE];
}

uint32_t CanSensorTinyBms::parseInt32(uint8_t* dataPtr) {
    return ((uint32_t)dataPtr[RSP_DATA_BYTE + 3] << 24) 
            | ((uint32_t)dataPtr[RSP_DATA_BYTE + 2] << 16)
            | ((uint32_t)dataPtr[RSP_DATA_BYTE + 1] << 8)
            | dataPtr[RSP_DATA_BYTE];
}

bool CanSensorTinyBms::_validate(uint8_t id) {
    return (millis() - _validationMap[id]) < STALE_INTERVAL;
}