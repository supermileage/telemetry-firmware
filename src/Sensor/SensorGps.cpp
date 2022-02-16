#include "SensorGps.h"
#include "settings.h"
#include "gpsGreenlist.h"

// GPS Update Frequency in Hz (1-10)
#define UPDATE_FREQ 4

// Math Constants
#define MICROSECONDS_IN_SECOND      1000000
#define NANOSECONDS_IN_MICROSECOND  1000
#define MILIMETERS_IN_METERS        1000.0
#define TEN_POWER_SEVEN             10000000.0
#define TEN_POWER_FIVE              100000.0

SensorGps::SensorGps(SFE_UBLOX_GNSS *gps) {
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
    _gps->setNavigationFrequency(UPDATE_FREQ);

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
        if(_speedCallback){
            _speedCallback(horizontalSpeed); 
            }       
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

    float value = _gps->getHorizontalAccEst() / MILIMETERS_IN_METERS;
    if (value > 0.0001 || value < 1000.0) {
        _valid = true;
    } else{
        _valid = false;
    }
        
}

bool SensorGps::getTimeValid() {
    return _gps->getTimeValid();
}

uint32_t SensorGps::getUnixTime() {
    return _gps->getUnixEpoch();
}

String SensorGps::getLongitude(bool &valid) {
    valid = false;
    double longitude = _gps->getLongitude() / TEN_POWER_SEVEN;
    double latitude = _gps->getLatitude() / TEN_POWER_SEVEN;

    if(_override) {
        valid = _valid;
    } else {
        for(positionBox p : GREEN_LIST) {
            if(p.isWithin(longitude, latitude)) {
                valid = _valid;
            }
        }
    }
    
    return FLOAT_TO_STRING(longitude, 6);
}

String SensorGps::getLatitude(bool &valid) {
    valid = false;

    double longitude = _gps->getLongitude() / TEN_POWER_SEVEN;
    double latitude = _gps->getLatitude() / TEN_POWER_SEVEN;

    if (_override) {
        valid = _valid;
    } else {
        for (positionBox p : GREEN_LIST) {
            if (p.isWithin(longitude, latitude)) {
                valid = _valid;
            }
        }
    }

    return FLOAT_TO_STRING(latitude, 6);
}

String SensorGps::getHeading(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_gps->getHeading() / TEN_POWER_FIVE, 5);    
}

String SensorGps::getHorizontalSpeed(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_gps->getGroundSpeed() / MILIMETERS_IN_METERS, 2);  
}

String SensorGps::getHorizontalAcceleration(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_horizontalAcceleration, 2);  
}

String SensorGps::getHorizontalAccuracy(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_gps->getHorizontalAccEst() / MILIMETERS_IN_METERS, 2);  
}

String SensorGps::getAltitude(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_gps->getAltitudeMSL() / MILIMETERS_IN_METERS, 2);  
}

String SensorGps::getVerticalSpeed(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_verticalSpeed, 2);  
}

String SensorGps::getVerticalAcceleration(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_verticalAcceleration, 2);  
}

String SensorGps::getVerticalAccuracy(bool &valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_gps->getVerticalAccEst() / MILIMETERS_IN_METERS, 2);  
}

int SensorGps::getSatellitesInView(bool &valid) {
    valid = true;
    return _gps->getSIV();  
}

void SensorGps::updateSpeedCallback(void (*speed)(float)){
   _speedCallback  = speed;
}

void SensorGps::toggleOverride() {
    _override = !_override;
}


