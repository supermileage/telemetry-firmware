#include "CanSensorSpeeduinoECUtemp.h"

CanSensorSpeeduinoECUtemp::CanSensorSpeeduinoECUtemp(CanInterface& canInterface) : CanListener(canInterface) { }

CanSensorSpeeduinoECUtemp::CanSensorSpeeduinoECUtemp(CanInterface& canInterface, uint16_t id) : CanListener(canInterface, id) { }

CanSensorSpeeduinoECUtemp::~CanSensorSpeeduinoECUtemp() { }

void CanSensorSpeeduinoECUtemp::handle() {
	if (_isAsleep)
		return;
	
	//uint64_t currentTime = millis();
    /* We won't use SoC update interval
	if (currentTime > _lastSocUpdate + SOC_UPDATE_INTERVAL) {
		_sendSocUpdate();
		_lastSocUpdate = currentTime;
	}*/
    
}

uint64_t CanSensorSpeeduinoECUtemp::getLastUpdateTime() {
	return _lastUpdateTime;
}

void CanSensorSpeeduinoECUtemp::setIsAsleep(bool value) {
	_isAsleep = value;
}

bool CanSensorSpeeduinoECUtemp::getIsAsleep() {
	return _isAsleep;
}

bool CanSensorSpeeduinoECUtemp::_validate(uint16_t id) {
    return (millis() - _validation_speeduinoMap[id]) < STALE_INTERVAL;
}



