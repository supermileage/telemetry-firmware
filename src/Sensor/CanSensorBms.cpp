#include "CanSensorBms.h"

CanSensorBms::CanSensorBms(CanInterface& canInterface) : CanListener(canInterface) {
	Handler::instance().remove(this);
}

CanSensorBms::CanSensorBms(CanInterface& canInterface, uint16_t id) : CanListener(canInterface, id) {
	Handler::instance().remove(this);
}

CanSensorBms::~CanSensorBms() { }

void CanSensorBms::setVoltageCallback(void (*callback)(float,float)) {
	_voltageCallback = callback;
}

bool CanSensorBms::_validate(uint16_t id) {
    return (millis() - _validationMap[id]) < STALE_INTERVAL;
}