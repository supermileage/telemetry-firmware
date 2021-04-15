#include "SensorGps.h"

char sentenceBuffer[128];
String completeSentence = "";

SensorGps::SensorGps(uint16_t updateInterval) {
    _updateInterval = updateInterval;
    _gps = new SFE_UBLOX_GNSS();
}

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

    _gps->setI2COutput(COM_TYPE_NMEA);
    _gps->setNMEAOutputPort(Serial);
    uint8_t rate = 1000 / _updateInterval;
    if(rate < 1) rate = 1;
    _gps->setNavigationFrequency(rate);

}

void SensorGps::handle() {
    _gps->checkUblox();
}

String SensorGps::getSentence() {
    return completeSentence;
}

void SFE_UBLOX_GNSS::processNMEA(char incoming) {
    if(incoming == '\n'){
        completeSentence = String(sentenceBuffer);
        memset(sentenceBuffer, 0, sizeof(sentenceBuffer));
    }else{
        uint8_t bufferLength = strlen(sentenceBuffer);
        sentenceBuffer[bufferLength] = incoming;
        sentenceBuffer[bufferLength+1] = '\0';
    }
}
