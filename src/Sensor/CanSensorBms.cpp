#include "CanSensorBms.h"

CanSensorBms::CanSensorBms(CanInterface& canInterface) : CanListener(canInterface) { }

CanSensorBms::CanSensorBms(CanInterface& canInterface, uint16_t id) : CanListener(canInterface, id) { }

CanSensorBms::~CanSensorBms() { }

void CanSensorBms::handle() {
	if (_isAsleep)
		return;
	
	uint64_t currentTime = millis();
	if (currentTime > _lastSocUpdate + SOC_UPDATE_INTERVAL) {
		_sendSocUpdate();
		_lastSocUpdate = currentTime;
	}
}

uint64_t CanSensorBms::getLastUpdateTime() {
	return _lastUpdateTime;
}

void CanSensorBms::setIsAsleep(bool value) {
	_isAsleep = value;
}

bool CanSensorBms::_validate(uint16_t id) {
    return (millis() - _validationMap[id]) < STALE_INTERVAL;
}

void CanSensorBms::_sendSocUpdate() {
		CanMessage message = { CAN_TELEMETRY_BMS_DATA, 0x8, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0} };
		memcpy((void*)message.data, (void*)&_soc, 4);
		memcpy((void*)(message.data + 4), (void*)&_batteryVoltage, 4);
		_canInterface.sendMessage(message);
}