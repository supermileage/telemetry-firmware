#include "SensorGps.h"

SensorGps::SensorGps(uint16_t updateInterval) {
    _updateInterval = updateInterval;
    _gps = new SFE_UBLOX_GNSS();
}

void SensorGps::begin() {
    Wire.begin();
    
    _gps->begin();

    _lastUpdate = millis();

    _gps->setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    _gps->saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
    _gps->setAutoPVT(true);
}

void SensorGps::handle() {
    
    if(millis() > _lastUpdate + _updateInterval) {
        _lastUpdate = millis();

        _latitude = _gps->getLatitude() * 0.0000001;
        _longitude = _gps->getLongitude() * 0.0000001;
        _altitude = _gps->getAltitude() * 0.001;
        _speed = _gps->getGroundSpeed() * 0.0036;
        _siv = _gps->getSIV();
    }
    
}

// Returns current latitude in degrees
double SensorGps::getLatitude() {
    return _latitude;
}

// Returns current longitude in degrees
double SensorGps::getLongitude() {
    return _longitude;
}

// Return current altitude in meters from sea level
double SensorGps::getAltitude() {
    return _altitude;
}

// Returns current ground speed in km/h
double SensorGps::getSpeed() {
    return _speed;
}

// Returns number of satellites in view
uint8_t SensorGps::getNumSatellites() {
    return _siv;
}

