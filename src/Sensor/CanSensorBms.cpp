#include "CanSensorBms.h"

CanSensorBms::CanSensorBms(CanInterface& canInterface) : CanListener(canInterface) { }

CanSensorBms::CanSensorBms(CanInterface& canInterface, uint16_t id) : CanListener(canInterface, id) { }

CanSensorBms::~CanSensorBms() { }

bool CanSensorBms::_validate(uint8_t id) {
    return (millis() - _validationMap[id]) < STALE_INTERVAL;
}