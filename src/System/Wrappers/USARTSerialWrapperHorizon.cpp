#include "USARTSerialWrapperHorizon.h"

USARTSerialWrapperHorizon::USARTSerialWrapperHorizon(USARTSerial *serial) : _usartSerial(serial) { }

USARTSerialWrapperHorizon::~USARTSerialWrapperHorizon() { }

void USARTSerialWrapperHorizon::begin(unsigned long baud, uint32_t flags) {
	_usartSerial->begin(baud, flags);
}

int USARTSerialWrapperHorizon::available() {
	return _usartSerial->available();
}

int USARTSerialWrapperHorizon::read() {
	return _usartSerial->read();
}

size_t USARTSerialWrapperHorizon::readBytes(char* buffer, size_t length) {
	return _usartSerial->readBytes(buffer, length);
}
