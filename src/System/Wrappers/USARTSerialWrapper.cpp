#include "USARTSerialWrapper.h"

USARTSerialWrapper::USARTSerialWrapper(USARTSerial *serial) : _usartSerial(serial) { }

USARTSerialWrapper::~USARTSerialWrapper() { }

void USARTSerialWrapper::begin(unsigned long baud, uint32_t flags) {
	_usartSerial->begin(baud, flags);
}

int USARTSerialWrapper::available() {
	return _usartSerial->available();
}

int USARTSerialWrapper::read() {
	return _usartSerial->read();
}

size_t USARTSerialWrapper::readBytes(char* buffer, size_t length) {
	return _usartSerial->readBytes(buffer, length);
}
