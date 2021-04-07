#include "Sensor_GPS.h"

char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

Sensor_GPS::Sensor_GPS(uint16_t updateInterval) {
    this->updateInterval = updateInterval;
}

void Sensor_GPS::begin() {
    Wire.begin();
    
    delay(3000);
    if(GPS.begin()) Serial.println("GPS initialized!");
    else Serial.println("Could not initialize GPS!");

    lastUpdate = millis();

    GPS.setI2COutput(COM_TYPE_NMEA); //Set the I2C port to output UBX only (turn off NMEA noise)
    GPS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
}

void Sensor_GPS::handle() {
    
    if(millis() > lastUpdate + updateInterval) {
        lastUpdate = millis(); //Update the timer

        latitude = GPS.getLatitude() * 0.0000001;
        longitude = GPS.getLongitude() * 0.0000001;
        altitude = GPS.getAltitude() * 0.001;
        speed = GPS.getGroundSpeed() * 0.0036;
    }
    
}

// Returns current latitude in degrees
double Sensor_GPS::getLatitude() {
    return latitude;
}

// Returns current longitude in degrees
double Sensor_GPS::getLongitude() {
    return longitude;
}

// Return current altitude in meters from sea level
double Sensor_GPS::getAltitude() {
    return altitude;
}

// Returns current ground speed in km/h
double Sensor_GPS::getSpeed() {
    return speed;
}

