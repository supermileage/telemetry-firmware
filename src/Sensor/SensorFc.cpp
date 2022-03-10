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
                Particle.publish("fcdata", _buffer, PRIVATE, WITH_ACK);
                _lastUpdate = millis();
            }
            
            memset(_buffer, 0, FC_BUFFER_SIZE);
        } else {
            uint16_t position = 0;
            while(_buffer[position]) {
                position++;
            }
            _buffer[position] = nextChar;
        }
    }

}
