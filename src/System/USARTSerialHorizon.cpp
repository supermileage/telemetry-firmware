#include "USARTSerialHorizon.h"

// Replace supermileage_fake_serial in tests with actual Horizon usart serial 

// Default constructor
USARTSerialHorizon::USARTSerialHorizon() : _serial(Serial1) { }

// Destructor
USARTSerialHorizon::~USARTSerialHorizon() { }

// Begin serial communication
// Horizon Baud is 9600, Flag is SERIAL_8N1
void USARTSerialHorizon::begin(unsigned long baud, uint32_t flags) {
    _serial.begin(baud, flags);
}

int USARTSerialHorizon::available() {
    return _serial.available();
}

int USARTSerialHorizon::read() {
    return _serial.read();
}

size_t USARTSerialHorizon::readBytes(char* buffer, size_t length) {
    size_t bytesRead = 0;
    while (bytesRead < length) {
        if (_serial.available()) {
            buffer[bytesRead++] = _serial.read();
        }
    }
    return bytesRead;


    // Fix this, implement to replace supermileage_fake_serial
}
