#include "SensorGps.h"

SensorGps::SensorGps(uint8_t updateFrequency) {
    _updateFrequency = updateFrequency;

    _gps = new SFE_UBLOX_GNSS();

    _lastUpdateMicros = 0;
    _lastXySpeed = 0.0;
    _xyAcceleration = 0.0;
    _lastZPosition = 0.0;
    _zSpeed = 0.0;
    _lastZSpeed = 0.0;
    _zAcceleration = 0.0;
}

String SensorGps::getHumanName() {
    return "GPS";
}

void SensorGps::begin() {
    Wire.begin();
    Wire.setClock(400000);

    _gps->begin();

    // Output NMEA and UBX messages over i2c
    _gps->setI2COutput(COM_TYPE_UBX);
    // GPS polls are non-blocking
    _gps->setAutoPVT(true);
    // Set the update frequency
    _gps->setNavigationFrequency(_updateFrequency);

}

void SensorGps::handle() {
    // The Sparkfun GNSS library automatically rate-limits the checks to _updateFrequency * 4
    _gps->checkUblox();

    // Calculate the current microsecond
    uint64_t thisUpdateMicros = (_gps->getUnixEpoch() * 1000000) + (_gps->getNanosecond() / 1000);

    // Check to see if there has been an update
    if(thisUpdateMicros != _lastUpdateMicros){

        uint64_t elapsedTime = thisUpdateMicros - _lastUpdateMicros;

        // Calculate XY Acceleration
        float xySpeed = _gps->getGroundSpeed() / 1000.0;
        _xyAcceleration = ((xySpeed - _lastXySpeed) * 1000000.0) / elapsedTime;
        _lastXySpeed = xySpeed;

        // Calculate Z Speed
        float zPosition = _gps->getAltitudeMSL() / 1000.0;
        _zSpeed = ((zPosition - _lastZPosition) * 1000000.0) / elapsedTime;
        _lastZPosition = zPosition;

        // Calculate Z Acceleration
        _zAcceleration = ((_zSpeed - _lastZSpeed) * 1000000.0) / elapsedTime;
        _lastZSpeed = _zSpeed;

        _lastUpdateMicros = thisUpdateMicros;
    }
        
}

bool SensorGps::getTimeValid() {
    return _gps->getTimeValid();
}

uint32_t SensorGps::getUnixTime() {
    return _gps->getUnixEpoch();
}

float SensorGps::getLongitude() {
    return _gps->getLongitude() / 10000000.0;
}

float SensorGps::getLatitude() {
    return _gps->getLatitude() / 10000000.0;
}

float SensorGps::getHeading() {
    return _gps->getHeading() / 100000.0;
}

float SensorGps::getXySpeed() {
    return _gps->getGroundSpeed() / 1000.0;
}

float SensorGps::getXyAcceleration() {
    return _xyAcceleration;
}

float SensorGps::getXyAccuracy() {
    return _gps->getHorizontalAccEst() / 1000.0;
}

float SensorGps::getAltitude() {
    return _gps->getAltitudeMSL() / 1000.0;
}

float SensorGps::getZSpeed() {
    return _zSpeed;
}

float SensorGps::getZAcceleration() {
    return _zAcceleration;
}

float SensorGps::getZAccuracy() {
    return _gps->getVerticalAccEst() / 1000.0;
}
