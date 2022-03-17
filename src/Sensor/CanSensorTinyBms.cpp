#include "CanSensorTinyBms.h"
#include "settings.h"

CanSensorTinyBms::CanSensorTinyBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanSensorBms(canInterface, CAN_TINYBMS_RESPONSE), _requestIntervalMs(requestIntervalMs) {

    }

void CanSensorTinyBms::handle() {
    if(millis() - _lastValidTime >= _requestIntervalMs) {

        CanMessage msg = CAN_MESSAGE_NULL;
        msg.id = CAN_TINYBMS_REQUEST;
        msg.dataLength = REQ_DATA_LENGTH;

        if(_bmsStatus == FaultError) {
            msg.data[0] = PARAM_ID_EVENTS;
            _canInterface.sendMessage(msg);
        }

        msg.data[0] = PARAM_IDS[_currentParam];
            
        _canInterface.sendMessage(msg);

        if(_currentParam == NUM_PARAMS - 1) {
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
				_voltageCallback(_soc, _batteryVoltage);
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
            case PARAM_ID_EVENTS:
                if(message.data[7] == 1 && message.data[6] <= 0x0F) {
                    _fault = message.data[6];
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

int CanSensorTinyBms::getFault(bool& valid) {
    valid = _validate(PARAM_ID_STATUS) && _validate(PARAM_ID_EVENTS);

    if(_bmsStatus == FaultError) {
        return _getFaultCode(_fault);
    }

    return BmsFault::NONE;
}

void CanSensorTinyBms::restart() {
    CanMessage msg = CAN_MESSAGE_NULL;
    msg.id = CAN_TINYBMS_REQUEST;
    msg.dataLength = REQ_DATA_LENGTH;
    msg.data[0] = PARAM_ID_RESET;
    msg.data[1] = RESET_ID_BMS;
    _canInterface.sendMessage(msg);
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

uint8_t CanSensorTinyBms::_getFaultCode(uint8_t fault) {
    switch(fault) {
        case FAULT_UNDER_VOLTAGE:
            return BmsFault::VOLTAGE_LOW;           
        case FAULT_OVER_VOLTAGE:
            return BmsFault::VOLTAGE_HIGH;         
        case FAULT_OVER_TEMP:
            return BmsFault::TEMP_HIGH;                   
        case FAULT_OVER_CURRENT_DISCHARGE: 
            return BmsFault::CURRENT_DISCHARGE_HIGH;
        case FAULT_OVER_CURRENT_CHARGE:
            return BmsFault::CURRENT_CHARGE_HIGH;
        case FAULT_OVER_CURRENT_REGEN:  
            return BmsFault::CURRENT_REGEN_HIGH;          
        case FAULT_LOW_TEMP:       
            return BmsFault::TEMP_LOW;               
        case FAULT_CHARGER_SWITCH:      
            return BmsFault::SWITCH_CHARGE;          
        case FAULT_LOAD_SWITCH: 
            return BmsFault::SWITCH_DISCHARGE;                  
        case FAULT_SINGLE_PORT_SWITCH:        
            return BmsFault::SWITCH_DISCHARGE;    
        case FAULT_CURRENT_SENSOR_DISCONNECTED: 
            return BmsFault::CURRENT_SENSOR_DISCONNECT;  
        case FAULT_CURRENT_SENSOR_CONNECTED:     
            return BmsFault::CURRENT_SENSOR_CONNECT; 
        default: 
            return BmsFault::NONE;               
    }
}
