#include "CanSensorSteering.h"
#include "settings.h"

#define STEERING_READY_BIT_IGNITION 0x00
#define STEERING_READY_BIT_DMS      0x01
#define STEERING_READY_BIT_BRAKE    0x02

CanSensorSteering::CanSensorSteering(CanInterface &canInterface) 
    : CanListener(canInterface) { }

void CanSensorSteering::begin() {
    _canInterface.addMessageListen(CAN_STEERING_THROTTLE, new CanListener::CanListenerDelegate(this));
    _canInterface.addMessageListen(CAN_STEERING_READY, new CanListener::CanListenerDelegate(this));
}

void CanSensorSteering::handle() {
    _validThrottle = (millis() - _lastUpdateThrottle) < STALE_INTERVAL;
    _validReady = (millis() - _lastUpdateReady) < STALE_INTERVAL;
}

String CanSensorSteering::getHumanName() {
    return "CanSensorSteering";
}

int CanSensorSteering::getThrottle(bool& valid) {
    valid = _validThrottle;
    return (_throttle * 100) / 255;
}

int CanSensorSteering::getIgnition(bool& valid) {
    valid = _validReady;
    return _ignition;
}

int CanSensorSteering::getDms(bool& valid) {
    valid = _validReady;
    return _dms;
}

int CanSensorSteering::getBrake(bool& valid) {
    valid = _validReady;
    return _brake;
}

void CanSensorSteering::update(CanMessage message) {

    if(message.id == CAN_STEERING_THROTTLE) {
        _lastUpdateThrottle = millis();
        _throttle = message.data[0];
    } else if(message.id == CAN_STEERING_READY) {
        _lastUpdateReady = millis();
        _ignition = (message.data[0] >> STEERING_READY_BIT_IGNITION) & 0x01;
        _dms = (message.data[0] >> STEERING_READY_BIT_DMS) & 0x01;
        _brake = (message.data[0] >> STEERING_READY_BIT_BRAKE) & 0x01;
    }

    
}
