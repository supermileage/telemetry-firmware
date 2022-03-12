#include "SensorFc.h"
#include "settings.h"

#define FC_BAUD         9600
#define FC_STOP_CHAR    '*'
#define PUBLISH_INTERVAL 5000

SensorFc::SensorFc(USARTSerial *serial) {
    _serial = serial;
}

String SensorFc::getHumanName() {
    return "FC Controller";
}

void SensorFc::begin() {
    _serial->begin(FC_BAUD);
}

void SensorFc::handle() {

    while(_serial->available()) {
        char nextChar = _serial->read();
        if(nextChar == FC_STOP_CHAR) {

            if(millis() >= _lastUpdate + PUBLISH_INTERVAL) {
                _lastUpdate = millis();
                _publish();
            }

            _emptyBuffer();

        } else {

            uint16_t position = 0;
            while(position < FC_BUFFER_SIZE && _buffer[position]) {
                position++;
            }

            if(position >= FC_BUFFER_SIZE) {
                DEBUG_SERIAL_LN("ERROR: FC SERIAL BUFFER OVERFLOW");
                _publish();
                _emptyBuffer();
            }

            _buffer[position] = nextChar;
        }
    }

}

void SensorFc::_publish() {
    DEBUG_SERIAL_LN("------------- FUEL CELL DATA:");
    DEBUG_SERIAL_LN(_buffer);
    DEBUG_SERIAL_LN();
    Particle.publish("fcdata", _buffer, PRIVATE, WITH_ACK);
}

void SensorFc::_emptyBuffer() {
    memset(_buffer, 0, FC_BUFFER_SIZE);
}
