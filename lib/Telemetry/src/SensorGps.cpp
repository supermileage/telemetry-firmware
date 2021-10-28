#include "SensorGps.h"

SensorGps::SensorGps(uint8_t updateFrequency) {
    _updateFrequency = updateFrequency;

    _gps = new SFE_UBLOX_GNSS();

    _lastUpdateMicros = 0;
    _lastHorizontalSpeed = 0.0;
    _horizontalAcceleration = 0.0;
    _lastAltitude = 0.0;
    _verticalSpeed = 0.0;
    _lastVerticalSpeed = 0.0;
    _verticalAcceleration = 0.0;
}

String SensorGps::getHumanName() {
    return "GPS";
}

void SensorGps::begin() {
    // Start i2c with clock speed of 400 KHz
    // This requires the pull-up resistors to be removed on i2c bus by cutting i2c jumpers on Sparkfun GNSS module
    Wire.setClock(400000);
    Wire.begin();
    
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
        float horizontalSpeed = _gps->getGroundSpeed() / 1000.0;
        _horizontalAcceleration = ((horizontalSpeed - _lastHorizontalSpeed) * 1000000.0) / elapsedTime;
        _lastHorizontalSpeed = horizontalSpeed;

        // Calculate Z Speed
        float altitude = _gps->getAltitudeMSL() / 1000.0;
        _verticalSpeed = ((altitude - _lastAltitude) * 1000000.0) / elapsedTime;
        _lastAltitude = altitude;

        // Calculate Z Acceleration
        _verticalAcceleration = ((_verticalSpeed - _lastVerticalSpeed) * 1000000.0) / elapsedTime;
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
    return _gps->getLongitude() / 10000000.0;
}

float SensorGps::getLatitude() {
    return _gps->getLatitude() / 10000000.0;
}

float SensorGps::getHeading() {
    return _gps->getHeading() / 100000.0;
}

float SensorGps::getHorizontalSpeed() {
    return _gps->getGroundSpeed() / 1000.0;
}

float SensorGps::getHorizontalAcceleration() {
    return _horizontalAcceleration;
}

float SensorGps::getHorizontalAccuracy() {
    return _gps->getHorizontalAccEst() / 1000.0;
}

float SensorGps::getAltitude() {
    return _gps->getAltitudeMSL() / 1000.0;
}

float SensorGps::getVerticalSpeed() {
    return _verticalSpeed;
}

float SensorGps::getVerticalAcceleration() {
    return _verticalAcceleration;
}

float SensorGps::getVerticalAccuracy() {
    return _gps->getVerticalAccEst() / 1000.0;
}
