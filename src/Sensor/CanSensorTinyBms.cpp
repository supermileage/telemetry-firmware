#include "CanSensorTinyBms.h"
#include "settings.h"

const uint8_t PARAM_IDS[] =  {
	TINYBMS_PARAM_ID_BATTERY_VOLTAGE,
	TINYBMS_PARAM_ID_BATTERY_CURRENT,
	TINYBMS_PARAM_ID_MAX_CELL_VOLTAGE,
	TINYBMS_PARAM_ID_MIN_CELL_VOLTAGE,
	TINYBMS_PARAM_ID_STATUS,
	TINYBMS_PARAM_ID_SOC,
	TINYBMS_PARAM_ID_TEMP
};

const uint8_t VALIDATION_IDS[] {
    TINYBMS_PARAM_ID_BATTERY_VOLTAGE,
    TINYBMS_PARAM_ID_BATTERY_CURRENT,
    TINYBMS_PARAM_ID_MAX_CELL_VOLTAGE,
    TINYBMS_PARAM_ID_MIN_CELL_VOLTAGE,
    TINYBMS_PARAM_ID_STATUS,
    TINYBMS_PARAM_ID_SOC,
    TINYBMS_PARAM_ID_TEMP,
    TINYBMS_TEMP_ID_INTERNAL,
    TINYBMS_TEMP_ID_BATTERY_1,
    TINYBMS_TEMP_ID_BATTERY_2,
    TINYBMS_PARAM_ID_EVENTS
};

const uint8_t* CanSensorTinyBms::ParamIds = PARAM_IDS;

const std::unordered_map<uint8_t, BmsFault::Code> CanSensorTinyBms::FaultCodeMap = {
    { TINYBMS_FAULT_UNDER_VOLTAGE, BmsFault::VOLTAGE_LOW },
    { TINYBMS_FAULT_OVER_VOLTAGE, BmsFault::VOLTAGE_HIGH },
    { TINYBMS_FAULT_OVER_TEMP, BmsFault::TEMP_HIGH },
    { TINYBMS_FAULT_OVER_CURRENT_DISCHARGE, BmsFault::CURRENT_DISCHARGE_HIGH },
    { TINYBMS_FAULT_OVER_CURRENT_CHARGE, BmsFault::CURRENT_CHARGE_HIGH },
    { TINYBMS_FAULT_LOW_TEMP, BmsFault::TEMP_LOW },
    { TINYBMS_FAULT_CHARGER_SWITCH, BmsFault::SWITCH_CHARGE },
    { TINYBMS_FAULT_LOAD_SWITCH, BmsFault::SWITCH_DISCHARGE },
    { TINYBMS_FAULT_SINGLE_PORT_SWITCH, BmsFault::SWITCH_DISCHARGE },
    { TINYBMS_FAULT_CURRENT_SENSOR_DISCONNECTED, BmsFault::CURRENT_SENSOR_DISCONNECT },
    { TINYBMS_FAULT_CURRENT_SENSOR_CONNECTED, BmsFault::CURRENT_SENSOR_CONNECT }
};


CanSensorTinyBms::CanSensorTinyBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanSensorBms(canInterface, CAN_TINYBMS_RESPONSE), _requestIntervalMs(requestIntervalMs) { }

void CanSensorTinyBms::handle() {
    if(millis() - _lastValidTime >= _requestIntervalMs) {
        CanMessage msg = CAN_MESSAGE_NULL;
        msg.id = CAN_TINYBMS_REQUEST;
        msg.dataLength = TINYBMS_REQ_DATA_LENGTH;

        if(_bmsStatus == FaultError) {
            msg.data[0] = TINYBMS_PARAM_ID_EVENTS;
            _canInterface.sendMessage(msg);
        }

        msg.data[0] = PARAM_IDS[_currentParam];
		
        _canInterface.sendMessage(msg);

		(++_currentParam) %= TINYBMS_NUM_PARAMS;
        _lastValidTime = millis();
    }

	CanSensorBms::handle();
}

void CanSensorTinyBms::begin() {
	CanListener::begin();

	for (uint16_t id : VALIDATION_IDS)  {
		_validationMap[id] = 0;
	}
}

String CanSensorTinyBms::getHumanName() {
    return "CanSensorTinyBms";
}

String CanSensorTinyBms::getBatteryVolt(bool& valid) {
    valid  = _validate(TINYBMS_PARAM_ID_BATTERY_VOLTAGE);
    return FLOAT_TO_STRING(_batteryVoltage, 1);
}

String CanSensorTinyBms::getBatteryCurrent(bool& valid) {
    valid  = _validate(TINYBMS_PARAM_ID_BATTERY_CURRENT);
    return FLOAT_TO_STRING(_batteryCurrent, 1);
}

String CanSensorTinyBms::getMinVolt(bool& valid) {
    valid  = _validate(TINYBMS_PARAM_ID_MIN_CELL_VOLTAGE);
    return FLOAT_TO_STRING(_cellVoltageMin, 3);
}

String CanSensorTinyBms::getMaxVolt(bool& valid) {
    valid  = _validate(TINYBMS_PARAM_ID_MAX_CELL_VOLTAGE);
    return FLOAT_TO_STRING(_cellVoltageMax, 3);
}

String CanSensorTinyBms::getAvgVolt(bool& valid) {
	valid = _validate(TINYBMS_PARAM_ID_MIN_CELL_VOLTAGE) && _validate(TINYBMS_PARAM_ID_MAX_CELL_VOLTAGE);
	return FLOAT_TO_STRING((_cellVoltageMin + _cellVoltageMax) / 2.0f, 3);
}

String CanSensorTinyBms::getSoc(bool& valid) {
    valid  = _validate(TINYBMS_PARAM_ID_SOC);
    return FLOAT_TO_STRING(_soc, 1); 
}

int CanSensorTinyBms::getTempBms(bool& valid) {
    valid  = _validate(TINYBMS_TEMP_ID_INTERNAL);
    return _tempBms;
}

int CanSensorTinyBms::getMinBatteryTemp(bool& valid) {
    valid  = _validate(TINYBMS_TEMP_ID_BATTERY_1)  && _validate(TINYBMS_TEMP_ID_BATTERY_2);
    return min(_batteryTemp1, _batteryTemp2);
}

int CanSensorTinyBms::getMaxBatteryTemp(bool& valid) {
    valid  = _validate(TINYBMS_TEMP_ID_BATTERY_1)  && _validate(TINYBMS_TEMP_ID_BATTERY_2);
    return max(_batteryTemp1, _batteryTemp2);
}

int CanSensorTinyBms::getAvgBatteryTemp(bool& valid) {
	valid  = _validate(TINYBMS_TEMP_ID_BATTERY_1) && _validate(TINYBMS_TEMP_ID_BATTERY_2);
    return (_batteryTemp1 + _batteryTemp2) / 2;
}

int CanSensorTinyBms::getStatusBms(bool& valid) {
    valid  = _validate(TINYBMS_PARAM_ID_STATUS);
    return _bmsStatus;
}

String CanSensorTinyBms::getStatusBmsString(bool& valid) {
    valid  = _validate(TINYBMS_PARAM_ID_STATUS);
    return String(BMS_STATUS_STRINGS[_bmsStatus]);
}

int CanSensorTinyBms::getFault(bool& valid) {
    valid = _validate(TINYBMS_PARAM_ID_STATUS) && _validate(TINYBMS_PARAM_ID_EVENTS);

    if(_bmsStatus == FaultError) {
        return _getFaultCode(_fault);
    }

    return BmsFault::NONE;
}

void CanSensorTinyBms::restart() {
    CanMessage msg = CAN_MESSAGE_NULL;
    msg.id = CAN_TINYBMS_REQUEST;
    msg.dataLength = TINYBMS_REQ_DATA_LENGTH;
    msg.data[0] = TINYBMS_PARAM_ID_RESET;
    msg.data[1] = TINYBMS_RESET_ID_BMS;
    _canInterface.sendMessage(msg);
}

void CanSensorTinyBms::update(CanMessage message) {
	_lastUpdateTime = millis();
	
    if(message.data[TINYBMS_RSP_STATUS_BYTE] != TRUE) {
        DEBUG_SERIAL_LN("Poor BMS Data Received");
    }
    else {
        uint8_t id = message.data[TINYBMS_RSP_PARAM_ID_BYTE];
        switch (id) {
            case TINYBMS_PARAM_ID_BATTERY_VOLTAGE:
                _batteryVoltage = parseFloat(message.data);
                break;
            case TINYBMS_PARAM_ID_BATTERY_CURRENT:
                // tiny bms returns -A for discharging and +A for charging: we want the inverse so flip sign here
                _batteryCurrent = -parseFloat(message.data);
                break;
            case TINYBMS_PARAM_ID_MAX_CELL_VOLTAGE:
                _cellVoltageMax = (float)parseInt16(message.data) / TINYBMS_CELL_VOLTAGE_SCALING_FACTOR; 
                break;
            case TINYBMS_PARAM_ID_MIN_CELL_VOLTAGE:
                _cellVoltageMin = (float)parseInt16(message.data) / TINYBMS_CELL_VOLTAGE_SCALING_FACTOR; 
                break;
            case TINYBMS_PARAM_ID_STATUS: {
                unsigned statusCode = parseInt16(message.data);
                if(statusCode == TINYBMS_STATUS_CHARGING) {
                    _bmsStatus = Charging;
                }
                else if(statusCode == TINYBMS_STATUS_CHARGED) {
                    _bmsStatus = Charged;
                }
                else if(statusCode == TINYBMS_STATUS_DISCHARGING) {
                    _bmsStatus = Discharging;
                }
                else if(statusCode == TINYBMS_STATUS_REGENERATION) {
                    _bmsStatus = Regeneration;
                }
                else if(statusCode == TINYBMS_STATUS_IDLE) {
                    _bmsStatus = Idle;
                }
                else
                {
                    _bmsStatus = FaultError;
                }
                break;
            }
            case TINYBMS_PARAM_ID_SOC:
                _soc = (float)parseInt32(message.data) / 1000000.0;
                break;
            case TINYBMS_PARAM_ID_TEMP:
                if(message.data[TINYBMS_TEMP_ID_BYTE] == TINYBMS_TEMP_ID_INTERNAL) {
                    _tempBms = parseInt16(message.data + 1) / TINYBMS_TEMP_SCALING_FACTOR;
                    _validationMap[TINYBMS_TEMP_ID_INTERNAL] = _lastUpdateTime;
                }
                else if(message.data[TINYBMS_TEMP_ID_BYTE] == TINYBMS_TEMP_ID_BATTERY_1) {
                    _batteryTemp1 = parseInt16(message.data + 1) / TINYBMS_TEMP_SCALING_FACTOR;
                    _validationMap[TINYBMS_TEMP_ID_BATTERY_1] = _lastUpdateTime;
                }
                else if(message.data[TINYBMS_TEMP_ID_BYTE] == TINYBMS_TEMP_ID_BATTERY_2) {
                    _batteryTemp2 = parseInt16(message.data + 1) / TINYBMS_TEMP_SCALING_FACTOR;
                    _validationMap[TINYBMS_TEMP_ID_BATTERY_2] = _lastUpdateTime;
                }
                break;
            case TINYBMS_PARAM_ID_EVENTS:
                if(message.data[7] == 1 && message.data[6] <= 0x0F) {
                    _fault = message.data[6];
                }
                break; 
            default:
                break;
        }

		// update validation map--exclude temp because it needs special handling for 3 cases
		if (id != TINYBMS_PARAM_ID_TEMP && _validationMap.find(id) != _validationMap.end()) {
			_validationMap[id] = _lastUpdateTime;
		}
    }
}

float CanSensorTinyBms::parseFloat(uint8_t* dataPtr) {
    float output;
    memcpy((void*)&output, (void*)(dataPtr + TINYBMS_RSP_DATA_BYTE), 4);
    return output;
}

int16_t CanSensorTinyBms::parseInt16(uint8_t* dataPtr) {
    return (int16_t)((uint16_t)dataPtr[TINYBMS_RSP_DATA_BYTE + 1] << 8)
            | dataPtr[TINYBMS_RSP_DATA_BYTE];
}

int32_t CanSensorTinyBms::parseInt32(uint8_t* dataPtr) {
    return (int32_t)((uint32_t)dataPtr[TINYBMS_RSP_DATA_BYTE + 3] << 24) 
            | ((uint32_t)dataPtr[TINYBMS_RSP_DATA_BYTE + 2] << 16)
            | ((uint32_t)dataPtr[TINYBMS_RSP_DATA_BYTE + 1] << 8)
            | dataPtr[TINYBMS_RSP_DATA_BYTE];
}

int CanSensorTinyBms::_getFaultCode(uint8_t fault) {
    if (FaultCodeMap.find(fault) != FaultCodeMap.end()) {
        return FaultCodeMap.at(fault);
    }
    return BmsFault::NONE;
}
