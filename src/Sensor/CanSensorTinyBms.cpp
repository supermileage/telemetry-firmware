#include "CanSensorTinyBms.h"
#include "settings.h"

#define NUM_PARAMS                  7
#define REQ_DATA_LENGTH     		8

#define PARAM_ID_BATTERY_VOLTAGE    0x14
#define PARAM_ID_BATTERY_CURRENT    0x15
#define PARAM_ID_MAX_CELL_VOLTAGE   0x16
#define PARAM_ID_MIN_CELL_VOLTAGE   0x17
#define PARAM_ID_STATUS             0x18
#define PARAM_ID_SOC                0x1A
#define PARAM_ID_TEMP               0x1B

#define PARAM_ID_EVENTS             0x11
#define PARAM_ID_RESET              0x02
#define RESET_ID_BMS                0x05

#define STATUS_CHARGING             0x91
#define STATUS_CHARGED              0x92
#define STATUS_DISCHARGING          0x93
#define STATUS_REGENERATION         0x96
#define STATUS_IDLE                 0x97
#define STATUS_FAULT_ERROR          0x9B

#define TEMP_ID_INTERNAL    		0x00
#define TEMP_ID_BATTERY_1   		0x01
#define TEMP_ID_BATTERY_2   		0x02

#define RSP_STATUS_BYTE     		0x0
#define RSP_PARAM_ID_BYTE   		0x1
#define RSP_DATA_BYTE       		0x2

#define FAULT_UNDER_VOLTAGE                 0x02
#define FAULT_OVER_VOLTAGE                  0x03
#define FAULT_OVER_TEMP                     0x04
#define FAULT_OVER_CURRENT_DISCHARGE        0x05
#define FAULT_OVER_CURRENT_CHARGE           0x06
#define FAULT_LOW_TEMP                      0x0A
#define FAULT_CHARGER_SWITCH                0x0B
#define FAULT_LOAD_SWITCH                   0x0C
#define FAULT_SINGLE_PORT_SWITCH            0x0D
#define FAULT_CURRENT_SENSOR_DISCONNECTED   0x0E
#define FAULT_CURRENT_SENSOR_CONNECTED      0x0F

const uint8_t PARAM_IDS[] =  {
	PARAM_ID_BATTERY_VOLTAGE,
	PARAM_ID_BATTERY_CURRENT,
	PARAM_ID_MAX_CELL_VOLTAGE,
	PARAM_ID_MIN_CELL_VOLTAGE,
	PARAM_ID_STATUS,
	PARAM_ID_SOC,
	PARAM_ID_TEMP
};

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
    TEMP_ID_BATTERY_2,
    PARAM_ID_EVENTS
};

CanSensorTinyBms::CanSensorTinyBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanSensorBms(canInterface, CAN_TINYBMS_RESPONSE), _requestIntervalMs(requestIntervalMs) {
		for (uint16_t id : VALIDATION_IDS)  {
        	_validationMap[id] = 0;
    	}
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

	CanSensorBms::handle();
}

String CanSensorTinyBms::getHumanName() {
    return "CanSensorTinyBms";
}

void CanSensorTinyBms::update(CanMessage message) {
	_lastUpdateTime = millis();
	
    if(message.data[RSP_STATUS_BYTE] != TRUE) {
        DEBUG_SERIAL_LN("Poor BMS Data Received");
    }
    else {
        uint8_t id = message.data[RSP_PARAM_ID_BYTE];
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
                    _validationMap[TEMP_ID_INTERNAL] = _lastUpdateTime;
                }
                else if(message.data[5] == TEMP_ID_BATTERY_1) {
                    _batteryTemp1 = parseInt16(message.data + 1) / 10;
                    _validationMap[TEMP_ID_BATTERY_1] = _lastUpdateTime;
                }
                else if(message.data[5] == TEMP_ID_BATTERY_2) {
                    _batteryTemp2 = parseInt16(message.data + 1) / 10;
                    _validationMap[TEMP_ID_BATTERY_2] = _lastUpdateTime;
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
			_validationMap[id] = _lastUpdateTime;
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
    return FLOAT_TO_STRING(_cellVoltageMax, 3);
}

String CanSensorTinyBms::getMinVolt(bool& valid) {
    valid  = _validate(PARAM_ID_MIN_CELL_VOLTAGE);
    return FLOAT_TO_STRING(_cellVoltageMin, 3);
}

String CanSensorTinyBms::getAvgVolt(bool& valid) {
	valid = _validate(PARAM_ID_MIN_CELL_VOLTAGE) && _validate(PARAM_ID_MAX_CELL_VOLTAGE);
	return FLOAT_TO_STRING((_cellVoltageMin + _cellVoltageMax) / 2.0f, 3);
}

String CanSensorTinyBms::getSoc(bool& valid) {
    valid  = _validate(PARAM_ID_SOC);
    return FLOAT_TO_STRING(_soc, 1); 
}

int CanSensorTinyBms::getTempBms(bool& valid) {
    valid  = _validate(TEMP_ID_INTERNAL);
    return _tempBms;
}

int CanSensorTinyBms::getMinBatteryTemp(bool& valid) {
    valid  = _validate(TEMP_ID_BATTERY_1)  && _validate(TEMP_ID_BATTERY_2);
    return min(_batteryTemp1, _batteryTemp2);
}

int CanSensorTinyBms::getMaxBatteryTemp(bool& valid) {
    valid  = _validate(TEMP_ID_BATTERY_1)  && _validate(TEMP_ID_BATTERY_2);
    return max(_batteryTemp1, _batteryTemp2);
}

int CanSensorTinyBms::getAvgBatteryTemp(bool& valid) {
	valid  = _validate(TEMP_ID_BATTERY_1) && _validate(TEMP_ID_BATTERY_2);
    return (_batteryTemp1 + _batteryTemp2) / 2;
}

int CanSensorTinyBms::getStatusBms(bool& valid) {
    valid  = _validate(PARAM_ID_STATUS);
    return _bmsStatus;
}

String CanSensorTinyBms::getStatusBmsString(bool& valid) {
    valid  = _validate(PARAM_ID_STATUS);
    return String(BMS_STATUS_STRINGS[_bmsStatus]);
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
