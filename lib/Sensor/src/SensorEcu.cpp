#include "SensorEcu.h"

/**
 * Constructor 
 * @param *serial bus receiving ECU data
 **/
SensorEcu::SensorEcu(USARTSerial *serial)
{
    this->_serial = serial;
    this->_time = 0;
    this->_rpm = 0;
    this->_map = 0;
    this->_tps = 0;
    this->_ect = 0;
    this->_iat = 0;
    this->_o2s = 0;
    this->_spark = 0;
    this->_fuelpw1 = 0;
    this->_fuelpw2 = 0;
    this->_ubadc = 0;
}

/**
 * Begins ECU receive
 * */
void SensorEcu::begin()
{
    this->_serial->begin(115200, SERIAL_8N1);
}

/**
 * Flushes all data out of Serial receive buffer
 * */
void SensorEcu::flush()
{
    while (this->_serial->available())
    {
        this->_serial->read();
    }
}

/**
 * Checks Serial buffer for incoming data. If a full ECU data frame is in buffer, parses and saves it
 * */
void SensorEcu::handle()
{
    if (this->_serial->available() < 27)
    {
        return;
    }
    uint8_t buffer[27];

    this->_serial->readBytes((char *)buffer, 27);

    uint8_t header1 = buffer[0] & 0xFF;
    uint8_t header2 = buffer[1] & 0xFF;
    uint8_t header3 = buffer[2] & 0xFF;

    uint8_t dataLengthField = buffer[3] & 0xFF;
    uint8_t serviceId = buffer[4] & 0xFF;

    if (
        header1 == 0x80 &&
        header2 == 0x8F &&
        header3 == 0xEA &&
        dataLengthField == 0x16 &&
        serviceId == 0x50)
    {
        // Data is valid
        this->_time = Time.now();
        this->_rpm = this->_interpretValue(buffer[6], buffer[7], 0.25, 0.0);
        this->_map = this->_interpretValue(buffer[8], buffer[9], 0.0039, 0.0);
        this->_tps = this->_interpretValue(buffer[10], buffer[11], 0.0015, 0.0);
        this->_ect = this->_interpretValue(buffer[12], buffer[13], 1, -40.0);
        this->_iat = this->_interpretValue(buffer[14], buffer[15], 1, -40.0);
        this->_o2s = this->_interpretValue(buffer[16], buffer[17], 0.0048, 0.0);
        this->_spark = this->_interpretValue(buffer[18], buffer[19], 0.5, 0.0);
        this->_fuelpw1 = this->_interpretValue(buffer[20], buffer[21], 0.001, 0.0);
        this->_fuelpw2 = this->_interpretValue(buffer[22], buffer[23], 0.001, 0.0);
        this->_ubadc = this->_interpretValue(buffer[24], buffer[25], 0.00625, 0.0);
    }
    else
    {
        this->flush();
    }
}

/**
 * @return Timestamp of last ECU receive
 * */
time_t SensorEcu::getTimestamp()
{
    return this->_time;
}

/**
 * @return Rotations Per Minute
 * */
float SensorEcu::getRPM()
{
    return this->_rpm;
}

/**
 * @return Mass Airflow Pressure??
 * */
float SensorEcu::getMap()
{
    return this->_map;
}

/**
 * @return ??
 * */
float SensorEcu::getTPS()
{
    return this->_tps;
}

/**
 * @return ??
 * */
float SensorEcu::getECT()
{
    return this->_ect;
}

/**
 * @return ??
 * */
float SensorEcu::getIAT()
{
    return this->_iat;
}

/**
 * @return Oxygen Sensor??
 * */
float SensorEcu::getO2S()
{
    return this->_o2s;
}

/**
 * @return Ignition Timing??
 * */
float SensorEcu::getSpark()
{
    return this->_spark;
}

/**
 * @return Fuel Pressure??
 * */
float SensorEcu::getFuelPW1()
{
    return this->_fuelpw1;
}

/**
 * @return Fuel Pressure??
 * */
float SensorEcu::getFuelPW2()
{
    return this->_fuelpw2;
}

/**
 * @return ??
 * */
float SensorEcu::getUbAdc()
{
    return this->_ubadc;
}

/**
 * Helper for interpreting raw values from ECU
 * @param high byte
 * @param low byte
 * @param factor ??
 * @param offset ??
 * @return Intepreted value
 * */
float SensorEcu::_interpretValue(uint8_t high, uint8_t low, float factor, float offset)
{
    return (float)((int)high * 256 + (int)low) * factor + offset;
}

