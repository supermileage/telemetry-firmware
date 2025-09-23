#include <cmath>

#include "SensorGps.h"
#include "settings.h"
#include "gpsGreenlist.h"

#define DEBUG_GPS

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

// Lets the NAV-ODO callback access this SensorGps
SensorGps* SensorGps::_instance = nullptr;

#if defined(DEBUG_GPS)
static int gpsDisplayCount = 0;
#endif

String SensorGps::getHumanName() {
    return "GPS";
}

void SensorGps::begin() {
    // Initialize GNSS over I2C
    _initialized = _gps->begin();
    if (!_initialized) return;

    // Use UBX over I2C
    _gps->setI2COutput(COM_TYPE_UBX);

    // Enable automatic NAV-PVT (driver caches latest PVT)
    _gps->setAutoPVT(true);

    // Set measurement/update rate
    _gps->setNavigationFrequency(UPDATE_FREQ);

    // Odometer (NAV-ODO) setup
    _instance = this; // for NAV-ODO callback
    _odoEnabled = enableOdometer(true, VAL_LAYER_ALL, defaultMaxWait);
    // Vehicle profile (1 = car by default) when supported
    (void)_gps->setVal16(UBLOX_CFG_ODO_PROFILE, 1, VAL_LAYER_ALL, defaultMaxWait);
    // Enable auto NAV-ODO and register callback
    _gps->setAutoNAVODOcallback(navOdoCallback);
    _gps->setAutoNAVODO(true);
    _odoAvailable = false;

    #ifdef DEBUG_GPS
    DEBUG_SERIAL_LN("GPS init: Success");
    DEBUG_SERIAL("GPS ODO: ");
    DEBUG_SERIAL_LN(_odoEnabled ? "enabled" : "disabled (will poll)" );
    #endif
}

void SensorGps::handle() {
    _gps->checkUblox();

    // If ODO not enabled or data isn't available, keep polling periodicaly
    if (!_odoAvailable) {
        unsigned long now = millis();
        if (!_odoEnabled) {
            // poll ODO occasionally
            if (now - _lastOdoPoll >= 1000UL) {
                _odoEnabled = enableOdometer(true, VAL_LAYER_ALL, defaultMaxWait);
                (void)_gps->getNAVODO();
                _lastOdoPoll = now;
            }
        } else {
            if (now - _lastOdoPoll >= 1000UL) {
                (void)_gps->getNAVODO();
                _lastOdoPoll = now;
            }
        }
    }

    // Calculate the current microsecond
    uint64_t thisUpdateMicros = (_gps->getUnixEpoch() * MICROSECONDS_IN_SECOND) + (_gps->getNanosecond() / NANOSECONDS_IN_MICROSECOND);

    // Check to see if there has been an update (gps data is updated UPDATE_FREQ times per second, so this returns true at that rate)
    if(thisUpdateMicros != _lastUpdateMicros){
		
        uint64_t elapsedMicroseconds = thisUpdateMicros - _lastUpdateMicros;

        // Calculate XY Acceleration
        float horizontalSpeed = _gps->getGroundSpeed() / MILIMETERS_IN_METERS;
        if (_speedCallback) {
            _speedCallback(horizontalSpeed); 
        }
        _horizontalAcceleration = ((horizontalSpeed - _lastHorizontalSpeed) * MICROSECONDS_IN_SECOND) / elapsedMicroseconds;
        _lastHorizontalSpeed = horizontalSpeed;
        _horizontalDistance = horizontalSpeed * elapsedMicroseconds / MICROSECONDS_IN_SECOND;

        // Calculate Z Speed
        float altitude = _gps->getAltitudeMSL() / MILIMETERS_IN_METERS;
        _verticalSpeed = ((altitude - _lastAltitude) * MICROSECONDS_IN_SECOND) / elapsedMicroseconds;
        _verticalDistance = altitude - _lastAltitude;
        _lastAltitude = altitude;

        // Calculate Z Acceleration
        _verticalAcceleration = ((_verticalSpeed - _lastVerticalSpeed) * MICROSECONDS_IN_SECOND) / elapsedMicroseconds;
        _lastVerticalSpeed = _verticalSpeed;

        _lastUpdateMicros = thisUpdateMicros;

        #ifdef DEBUG_GPS
        if (gpsDisplayCount++ % 10 == 0) {
            double lon = _gps->getLongitude() / TEN_POWER_SEVEN;
            double lat = _gps->getLatitude() / TEN_POWER_SEVEN;
            DEBUG_SERIAL("GPS: ");
            DEBUG_SERIAL(FLOAT_TO_STRING(lat, 6));
            DEBUG_SERIAL(",");
            DEBUG_SERIAL(FLOAT_TO_STRING(lon, 6));
            DEBUG_SERIAL(" | ODO_CFG:");
            DEBUG_SERIAL(_odoEnabled ? "ok" : "fail");
            DEBUG_SERIAL(" | ODO: ");
            if (_odoAvailable) {
                DEBUG_SERIAL("Trip=");
                DEBUG_SERIAL(String(_odoDistance));
                DEBUG_SERIAL("m, Total=");
                DEBUG_SERIAL(String(_odoTotalDistance));
                DEBUG_SERIAL("m, +/-");
                DEBUG_SERIAL(String(_odoDistanceStd));
                DEBUG_SERIAL_LN("m");
            } else {
                DEBUG_SERIAL_LN("waiting for data");
            }
        }
        #endif
    }

    float value = _gps->getHorizontalAccEst() / MILIMETERS_IN_METERS;
    if (value > 0.0001 && value < 1000.0) {
        _valid = true;
    } else{
        _valid = false;
    }
        
}

bool SensorGps::getTimeValid() {
    return _gps->getTimeValid() && _initialized;
}

uint32_t SensorGps::getUnixTime() {
    return _gps->getUnixEpoch();
}

String SensorGps::getLongitude(bool &valid) {
    valid = false;
    double longitude = _gps->getLongitude() / TEN_POWER_SEVEN;
    double latitude = _gps->getLatitude() / TEN_POWER_SEVEN;

    if(_override) {
        valid = _valid && _initialized;
    } else {
        for(positionBox p : GREEN_LIST) {
            if(p.isWithin(longitude, latitude)) {
                valid = _valid && _initialized;
                break;
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
        valid = _valid && _initialized;
    } else {
        for (positionBox p : GREEN_LIST) {
            if (p.isWithin(longitude, latitude)) {
                valid = _valid && _initialized;
                break;
            }
        }
    }

    return FLOAT_TO_STRING(latitude, 6);
}

int SensorGps::getHeading(bool &valid) {
    valid = _valid && _initialized;
    return _gps->getHeading() / TEN_POWER_FIVE;    
}

String SensorGps::getHorizontalSpeed(bool &valid) {
    valid = _valid && _initialized;
    return FLOAT_TO_STRING(_gps->getGroundSpeed() / MILIMETERS_IN_METERS, 2);  
}

String SensorGps::getHorizontalAcceleration(bool &valid) {
    valid = _valid && _initialized;
    return FLOAT_TO_STRING(_horizontalAcceleration, 2);  
}

String SensorGps::getHorizontalAccuracy(bool &valid) {
    valid = _valid && _initialized;
    float value = _gps->getHorizontalAccEst() / MILIMETERS_IN_METERS;
    if (value > 1000.0){
        return "1000.00";
    }
    return FLOAT_TO_STRING(value, 2);  
}

String SensorGps::getAltitude(bool &valid) {
    valid = _valid && _initialized;
    return FLOAT_TO_STRING(_gps->getAltitudeMSL() / MILIMETERS_IN_METERS, 2);  
}

String SensorGps::getVerticalSpeed(bool &valid) {
    valid = _valid && _initialized;
    return FLOAT_TO_STRING(_verticalSpeed, 2);  
}

String SensorGps::getVerticalAcceleration(bool &valid) {
    valid = _valid && _initialized;
    return FLOAT_TO_STRING(_verticalAcceleration, 2);  
}

String SensorGps::getVerticalAccuracy(bool &valid) {
    valid = _valid && _initialized;
    float value = _gps->getVerticalAccEst() / MILIMETERS_IN_METERS;
    if (value > 1000.0){
        return "1000.00";
    }
    return FLOAT_TO_STRING(value, 2);  
}

String SensorGps::getIncline(bool &valid) {
    valid = true;
    double inclineInRadians = atan(_verticalDistance / _horizontalDistance);
    _verticalDistance = 0;
	_horizontalDistance = 0;
    return FLOAT_TO_STRING(degrees(inclineInRadians), 2);
}

int SensorGps::getSatellitesInView(bool &valid) {
    valid = true;
    return _gps->getSIV();
}

// NAV-ODO callback: cache latest values
void SensorGps::navOdoCallback(UBX_NAV_ODO_data_t data) {
    if (_instance != nullptr) {
        _instance->_odoDistance = data.distance;
        _instance->_odoTotalDistance = data.totalDistance;
        _instance->_odoDistanceStd = data.distanceStd;
        bool wasAvailable = _instance->_odoAvailable;
        _instance->_odoAvailable = true;
        #ifdef DEBUG_GPS
        if (!wasAvailable) {
            DEBUG_SERIAL_LN("GPS ODO: first data received");
        }
        #endif
    }
}

bool SensorGps::enableOdometer(bool enable, uint8_t layer, uint16_t maxWait) {
    // Write UBLOX_CFG_ODO_USE_ODO
    uint8_t val = enable ? 1 : 0;
    uint8_t status = _gps->setVal8(UBLOX_CFG_ODO_USE_ODO, val, layer, maxWait);
    return (status == SFE_UBLOX_STATUS_SUCCESS) || (status == SFE_UBLOX_STATUS_DATA_SENT);
}

String SensorGps::getOdoDistance(bool &valid) {
    valid = _valid && _initialized && _odoAvailable;
    return FLOAT_TO_STRING(static_cast<float>(_odoDistance), 2);
}

String SensorGps::getOdoTotalDistance(bool &valid) {
    valid = _valid && _initialized && _odoAvailable;
    return FLOAT_TO_STRING(static_cast<float>(_odoTotalDistance), 2);
}

String SensorGps::getOdoDistanceStd(bool &valid) {
    valid = _valid && _initialized && _odoAvailable;
    return FLOAT_TO_STRING(static_cast<float>(_odoDistanceStd), 2);
}

bool SensorGps::resetOdometer() {
    // Reset distance on the receiver; clear local cache
    bool ok = _gps->resetOdometer();
    if (ok) {
        _odoDistance = 0;
        _odoDistanceStd = 0;
        _odoAvailable = false; // repopulates on next NAV-ODO
    }
    return ok;
}

void SensorGps::setSpeedCallback(void (*speed)(float)){
   _speedCallback  = speed;
}

void SensorGps::toggleOverride() {
    _override = !_override;
}

String SensorGps::getInitStatus() {
	return _initialized ? "Success" : "Failure";
}


