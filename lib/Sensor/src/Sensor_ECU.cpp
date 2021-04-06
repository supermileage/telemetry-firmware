#include "Sensor_ECU.h"

Sensor_ECU::Sensor_ECU(USARTSerial *serial)
{
    this->_serial = serial;
    this->_time = 0;
}

void Sensor_ECU::begin()
{
    this->_serial->begin(115200, SERIAL_8N1);
}

void Sensor_ECU::flush()
{
    while (this->_serial->available())
    {
        this->_serial->read();
    }
}

void Sensor_ECU::handle()
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
        fields["rpm"] = this->_interpretValue(buffer[6], buffer[7], 0.25, 0.0);
        fields["map"] = this->_interpretValue(buffer[8], buffer[9], 0.0039, 0.0);
        fields["tps"] = this->_interpretValue(buffer[10], buffer[11], 0.0015, 0.0);
        fields["ect"] = this->_interpretValue(buffer[12], buffer[13], 1, -40.0);
        fields["iat"] = this->_interpretValue(buffer[14], buffer[15], 1, -40.0);
        fields["o2s"] = this->_interpretValue(buffer[16], buffer[17], 0.0048, 0.0);
        fields["spark"] = this->_interpretValue(buffer[18], buffer[19], 0.5, 0.0);
        fields["fuelpw1"] = this->_interpretValue(buffer[20], buffer[21], 0.001, 0.0);
        fields["fuelpw2"] = this->_interpretValue(buffer[22], buffer[23], 0.001, 0.0);
        fields["ubadc"] = this->_interpretValue(buffer[24], buffer[25], 0.00625, 0.0);
    }
    else
    {
        this->flush();
    }
}

float Sensor_ECU::get(String field) {
    if(fields.find(field) != fields.end()) {
        return fields[field];
    }

    return 0;
}

time_t Sensor_ECU::getTimestamp()
{
    return this->_time;
}

float Sensor_ECU::_interpretValue(uint8_t high, uint8_t low, float factor, float offset)
{
    return (float)((int)high * 256 + (int)low) * factor + offset;
}