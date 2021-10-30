#include "SensorGps.h"

SensorGps::SensorGps(uint8_t updateFrequency) {
    _updateFrequency = updateFrequency;
    _gps = new SFE_UBLOX_GNSS();

}

SensorGps::SensorGps(const SensorGps &other){
    _updateFrequency = other._updateFrequency;
    _gps = new SFE_UBLOX_GNSS();
}

SensorGps& SensorGps::operator = (const SensorGps &other){
    if(this == &other) return *this;
    _updateFrequency = other._updateFrequency;
    _gps = other._gps;
    return *this;
}

SensorGps::~SensorGps(){
    delete _gps;
}

String SensorGps::getHumanName() {
    return "GPS";
}

void SensorGps::begin() {
 
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
    uint64_t thisUpdateMicros = (_gps->getUnixEpoch() * MICROSECONDS_IN_SECOND) + (_gps->getNanosecond() / NANOSECONDS_IN_MICROSECOND);

    // Check to see if there has been an update
    if(thisUpdateMicros != _lastUpdateMicros){

        uint64_t elapsedMicroseconds = thisUpdateMicros - _lastUpdateMicros;

        // Calculate XY Acceleration
        float horizontalSpeed = _gps->getGroundSpeed() / MILIMETERS_IN_METERS;
        _horizontalAcceleration = ((horizontalSpeed - _lastHorizontalSpeed) * MICROSECONDS_IN_SECOND) / elapsedMicroseconds;
        _lastHorizontalSpeed = horizontalSpeed;

        // Calculate Z Speed
        float altitude = _gps->getAltitudeMSL() / MILIMETERS_IN_METERS;
        _verticalSpeed = ((altitude - _lastAltitude) * MICROSECONDS_IN_SECOND) / elapsedMicroseconds;
        _lastAltitude = altitude;

        // Calculate Z Acceleration
        _verticalAcceleration = ((_verticalSpeed - _lastVerticalSpeed) * MICROSECONDS_IN_SECOND) / elapsedMicroseconds;
        _lastVerticalSpeed = _verticalSpeed;

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
    return _gps->getLongitude() / TEN_POWER_SEVEN;
}

float SensorGps::getLatitude() {
    return _gps->getLatitude() / TEN_POWER_SEVEN;
}

float SensorGps::getHeading() {
    return _gps->getHeading() / TEN_POWER_FIVE;
}

float SensorGps::getHorizontalSpeed() {
    return _gps->getGroundSpeed() / MILIMETERS_IN_METERS;
}

float SensorGps::getHorizontalAcceleration() {
    return _horizontalAcceleration;
}

float SensorGps::getHorizontalAccuracy() {
    return _gps->getHorizontalAccEst() / MILIMETERS_IN_METERS;
}

float SensorGps::getAltitude() {
    return _gps->getAltitudeMSL() / MILIMETERS_IN_METERS;
}

float SensorGps::getVerticalSpeed() {
    return _verticalSpeed;
}

float SensorGps::getVerticalAcceleration() {
    return _verticalAcceleration;
}

float SensorGps::getVerticalAccuracy() {
    return _gps->getVerticalAccEst() / MILIMETERS_IN_METERS;
}
