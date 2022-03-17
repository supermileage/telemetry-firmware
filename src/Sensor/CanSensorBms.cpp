#include "CanSensorBms.h"

const char* BMS_STATUSES[7] = { "Charging...", "Charged!", "Discharging...", "Regeneration", "Idle", "Fault Error", "Unknown" };

// these are from TinyBms's param IDs, used here as arbitrary IDs for all BMS
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

CanSensorBms::CanSensorBms(CanInterface& canInterface) : CanListener(canInterface) {
	for (auto id : VALIDATION_IDS)  {
        _validationMap[id] = 0;
    }
}

CanSensorBms::CanSensorBms(CanInterface& canInterface, uint16_t id) : CanListener(canInterface, id) { }

CanSensorBms::~CanSensorBms() { }

String CanSensorBms::getBatteryVolt(bool& valid) {
    valid  = _validate(PARAM_ID_BATTERY_VOLTAGE);
    return FLOAT_TO_STRING(_batteryVoltage, 1);
}

String CanSensorBms::getBatteryCurrent(bool& valid) {
    valid  = _validate(PARAM_ID_BATTERY_CURRENT);
    return FLOAT_TO_STRING(_batteryCurrent, 1);
}

String CanSensorBms::getMaxVolt(bool& valid) {
    valid  = _validate(PARAM_ID_MAX_CELL_VOLTAGE);
    return FLOAT_TO_STRING(_cellVoltageMax, 2);
}

String CanSensorBms::getMinVolt(bool& valid) {
    valid  = _validate(PARAM_ID_MIN_CELL_VOLTAGE);
    return FLOAT_TO_STRING(_cellVoltageMin, 2);
}

String CanSensorBms::getSoc(bool& valid) {
    valid  = _validate(PARAM_ID_SOC);
    return FLOAT_TO_STRING(_soc, 1); 
}

int CanSensorBms::getTempBms(bool& valid) {
    valid  = _validate(TEMP_ID_INTERNAL);
    return _tempBms;
}

int CanSensorBms::getBatteryTemp1(bool& valid) {
    valid  = _validate(TEMP_ID_BATTERY_1);
    return _batteryTemp1;
}

int CanSensorBms::getBatteryTemp2(bool& valid) {
    valid  = _validate(TEMP_ID_BATTERY_2);
    return _batteryTemp2;
}

int CanSensorBms::getStatusBms(bool& valid) {
    valid  = _validate(PARAM_ID_STATUS);
    return _bmsStatus;
}

String CanSensorBms::getStatusBmsString(bool& valid) {
    valid  = _validate(PARAM_ID_STATUS);
    return String(BMS_STATUSES[_bmsStatus]);
}

void CanSensorBms::setVoltageCallback(void (*callback)(float,float)) {
	_voltageCallback = callback;
}

bool CanSensorBms::_validate(uint8_t id) {
    return (millis() - _validationMap[id]) < STALE_INTERVAL;
}