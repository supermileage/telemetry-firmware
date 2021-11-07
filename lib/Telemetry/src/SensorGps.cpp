#include "SensorGps.h"

SensorGps::SensorGps(SFE_UBLOX_GNSS *gps, uint8_t updateFrequency) {
    _updateFrequency = updateFrequency;
    _gps = gps;

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

String SensorGps::getLongitude() {
    return String::format("%.7f",_gps->getLongitude() / TEN_POWER_SEVEN);
}

String SensorGps::getLatitude() {
    return String::format("%.7f",_gps->getLatitude() / TEN_POWER_SEVEN);
}

String SensorGps::getHeading() {
    return String::format("%.5f",_gps->getHeading() / TEN_POWER_FIVE);    
}

String SensorGps::getHorizontalSpeed() {
    return String::format("%.2f",_gps->getGroundSpeed() / MILIMETERS_IN_METERS);  
}

String SensorGps::getHorizontalAcceleration() {
    return String::format("%.2f",_horizontalAcceleration);  
}

String SensorGps::getHorizontalAccuracy() {
    return String::format("%.2f",_gps->getHorizontalAccEst() / MILIMETERS_IN_METERS);  
}

String SensorGps::getAltitude() {
    return String::format("%.2f",_gps->getAltitudeMSL() / MILIMETERS_IN_METERS);  
}

String SensorGps::getVerticalSpeed() {
    return String::format("%.2f",_verticalSpeed);  
}

String SensorGps::getVerticalAcceleration() {
    return String::format("%.2f",_verticalAcceleration);  
}

String SensorGps::getVerticalAccuracy() {
    return String::format("%.2f",_gps->getVerticalAccEst() / MILIMETERS_IN_METERS);  
}

String SensorGps::getSatellitesInView() {
    return String::format("%d",_gps->getSIV());  
}
