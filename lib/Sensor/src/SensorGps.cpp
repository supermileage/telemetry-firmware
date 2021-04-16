#include "SensorGps.h"

char sentenceBuffer[128];
String completeSentence = "";

/**
 * Constructor 
 * @param updateInterval for GPS position/speed in ms
 **/
SensorGps::SensorGps(uint16_t updateInterval) {
    _updateInterval = updateInterval;
    _gps = new SFE_UBLOX_GNSS();
}

/**
 * Begin the GPS sensor by setting up over i2c
 **/
void SensorGps::begin() {
    Wire.begin();
    _gps->begin();

    // Disable all NMEA messages we don't want
    _gps->disableNMEAMessage(UBX_NMEA_GLL, COM_PORT_I2C);
    _gps->disableNMEAMessage(UBX_NMEA_GSA, COM_PORT_I2C);
    _gps->disableNMEAMessage(UBX_NMEA_GSV, COM_PORT_I2C);
    _gps->disableNMEAMessage(UBX_NMEA_GGA, COM_PORT_I2C);
    _gps->disableNMEAMessage(UBX_NMEA_VTG, COM_PORT_I2C);
    _gps->disableNMEAMessage(UBX_NMEA_TXT, COM_PORT_I2C);
    // Enable only GPRMC/GNRMC messages
    _gps->enableNMEAMessage(UBX_NMEA_RMC, COM_PORT_I2C);

    // Set i2c to only output NMEA messages
    _gps->setI2COutput(COM_TYPE_NMEA);

    // Convert update interval in ms to update rate in s
    // Limit update rate to 1Hz-10Hz
    uint8_t rate = 1000 / _updateInterval;
    if(rate < 1) rate = 1;
    else if(rate > 10) rate = 10;
    _gps->setNavigationFrequency(rate);

}

/**
 * Polls GPS for any new data
 **/
void SensorGps::handle() {
    _gps->checkUblox();
}

/**
 * @return GPRMC/GNRMC sentence containing position and other data
 **/
String SensorGps::getSentence() {
    return completeSentence;
}

/**
 * @return Speed in Kilometers per Hour
 **/
float SensorGps::getSpeedKph() {
    String speedString = completeSentence.substring(completeSentence.indexOf(",W,") + 3);
    speedString = speedString.substring(0,speedString.indexOf(','));
    float speedKnots = speedString.toFloat();
    return speedKnots * 1.852;
}

/**
 * Replaces the Sparkfun GNSS library method for handling incoming bytes over i2c
 * 
 * @param incoming byte over i2c
 **/
void SFE_UBLOX_GNSS::processNMEA(char incoming) {
    // If there is a new line, update the current complete sentence with buffer contents and clear the buffer
    if(incoming == '\n'){
        completeSentence = String(sentenceBuffer);
        memset(sentenceBuffer, 0, sizeof(sentenceBuffer));
    // Append the current byte to the buffer
    }else{
        uint8_t bufferLength = strlen(sentenceBuffer);
        sentenceBuffer[bufferLength] = incoming;
        sentenceBuffer[bufferLength+1] = '\0';
    }
}
