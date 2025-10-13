#include <cmath>

#include "SensorGps.h"
#include "settings.h"
#include "gpsGreenlist.h"

#define DEBUG_GPS

// Math Constants
#define MICROSECONDS_IN_SECOND      1000000
#define NANOSECONDS_IN_MICROSECOND  1000
#define MILIMETERS_IN_METERS        1000.0
#define TEN_POWER_SEVEN             10000000.0
#define TEN_POWER_FIVE              100000.0

SensorGps::SensorGps(SFE_UBLOX_GNSS *gps) {
    _gps = gps;
    _instance = this;
}

// Lets the NAV-ODO callback access this SensorGps
SensorGps* SensorGps::_instance = nullptr;

#if defined(DEBUG_GPS)
static int gpsDebugCounter = 0;
#endif

String SensorGps::getHumanName() {
    return "GPS";
}

void SensorGps::begin() {
    // Initialize GNSS over I2C
    _initialized = _gps->begin();
    _instance = this; // for NAV-ODO callback
    if (!_initialized) return;

    // Use UBX over I2C
    _gps->setI2COutput(COM_TYPE_UBX);

    _gps->saveConfigSelective(VAL_CFG_SUBSEC_IOPORT);

    // Enable automatic NAV-PVT (driver caches latest PVT)
    _gps->setAutoPVT(true);

    // Set measurement/update rate
    _gps->setNavigationFrequency(NAV_FREQ);

    _odoEnabled = enableOdometer(true, VAL_LAYER_ALL, MAX_WAIT);

    #ifdef DEBUG_GPS
    DEBUG_SERIAL_LN("GPS init: Success");
    #endif
}

void SensorGps::handle() {
    _gps->checkUblox();
    _gps->checkCallbacks();

    // Calculate the current microsecond
    uint64_t thisUpdateMicros = (_gps->getUnixEpoch() * MICROSECONDS_IN_SECOND) + (_gps->getNanosecond() / NANOSECONDS_IN_MICROSECOND);
    // Check to see if there has been an update
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
        if (gpsDebugCounter++ % 10 == 0) {
            double lon = _gps->getLongitude() / TEN_POWER_SEVEN;
            double lat = _gps->getLatitude() / TEN_POWER_SEVEN;
            DEBUG_SERIAL("GPS: ");
            DEBUG_SERIAL(FLOAT_TO_STRING(lat, 6));
            DEBUG_SERIAL(",");
            DEBUG_SERIAL(FLOAT_TO_STRING(lon, 6));
            DEBUG_SERIAL(" | ODO:");
            DEBUG_SERIAL(_odoEnabled ? "ok" : "fail");
            if (_odoAvailable) {
                DEBUG_SERIAL(", Trip=");
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

    // Retry odometer enabled 
    if (!_odoEnabled && _initialized) {
        uint64_t now = millis();

        // Initialize first retry delay if not set
        if (odoRetryNextMs == 0) {
            odoRetryNextMs = now + ODO_BASE_BACKOFF_MS;
        }

        if (odoRetryCount < ODO_MAX_RETRIES && now >= odoRetryNextMs) {
            #ifdef DEBUG_GPS
            DEBUG_SERIAL("Attempting odometer enable retry #");
            DEBUG_SERIAL_LN((int)odoRetryCount + 1);
            #endif

            bool success = enableOdometer(true, VAL_LAYER_ALL, defaultMaxWait);
            if (success) {
                _odoEnabled = true;

                #ifdef DEBUG_GPS
                DEBUG_SERIAL_LN("ODO enable retry succeeded!");
                #endif
            } else {
                scheduleNextOdoRetry(now);
            }
        }
    }
}

void SensorGps::scheduleNextOdoRetry(uint64_t now) {
    odoRetryCount++;
    uint64_t backoff = ODO_BASE_BACKOFF_MS * (1UL << (odoRetryCount - 1)); // Exponential backoff
    backoff += (now & 255); // Add jitter
    odoRetryNextMs = now + backoff;

    #ifdef DEBUG_GPS
    DEBUG_SERIAL("Scheduled next ODO retry in ms=");
    DEBUG_SERIAL_LN((unsigned long)backoff);
    #endif
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
void SensorGps::navOdoCallback(UBX_NAV_ODO_data_t *ubxDataStruct) {
    if (_instance != nullptr) {
        _instance->_odo_iTOW = ubxDataStruct->iTOW;
        _instance->_odoDistance = ubxDataStruct->distance;
        _instance->_odoTotalDistance = ubxDataStruct->totalDistance;
        _instance->_odoDistanceStd = ubxDataStruct->distanceStd;
        _instance->_odoAvailable = true;
    }
    // #ifdef DEBUG_GPS
    // DEBUG_SERIAL("NAV-ODO callback: iTOW=");
    // DEBUG_SERIAL(String(ubxDataStruct->iTOW));
    // DEBUG_SERIAL(", distance=");
    // DEBUG_SERIAL(String(ubxDataStruct->distance));
    // DEBUG_SERIAL(", totalDistance=");
    // DEBUG_SERIAL(String(ubxDataStruct->totalDistance));
    // DEBUG_SERIAL(", distanceStd=");
    // DEBUG_SERIAL(String(ubxDataStruct->distanceStd));
    // DEBUG_SERIAL_LN("");
    // #endif
}

bool SensorGps::enableOdometer(bool enable, uint8_t layer, uint16_t maxWait) {
    // Set odometer profile and flags (car profile, enable odometer)
    uint8_t statusEnable = _gps->setVal(UBLOX_CFG_ODO_USE_ODO, (uint8_t)enable, layer, maxWait);
    //uint8_t statusProfile = _gps->setVal8(UBLOX_CFG_ODO_PROFILE, VEHICLE_PROFILE, layer, maxWait);

    // Register callback and enable automatic NAV-ODO messages only when enabling.
    // When disabling, turn off automatic NAV-ODO reports and clear local cache.
    if (enable) {
        _gps->setAutoNAVODOcallbackPtr(&SensorGps::navOdoCallback);
        //_gps->setAutoNAVODO(true);
    } else {
        // Disable automatic NAV-ODO messages from the GNSS. Use the library call to stop auto reports.
        //_gps->setAutoNAVODO(false);
        // Attempt to clear any registered callback pointer (library supports setting pointer to NULL).
        _gps->setAutoNAVODOcallbackPtr(nullptr);
        // Clear cached odometer data locally so stale callback data isn't used.
        _odoAvailable = false;
        _odoDistance = 0;
        _odoTotalDistance = 0;
        _odoDistanceStd = 0;
    }

    DEBUG_SERIAL("odoEnabled: ");
    DEBUG_SERIAL_LN(statusEnable);
    DEBUG_SERIAL("odoProfile: ");
    // DEBUG_SERIAL_LN(statusProfile);

    return (bool)statusEnable;
    // && (bool)statusProfile;
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
    _odoDistance = 0;
    _odoDistanceStd = 0;
    _odoAvailable = false; // repopulates on next NAV-ODO
    return true; //FIX this later
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


