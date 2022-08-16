#include "TelemetrySerialUSART.h"

TelemetrySerialUSART::TelemetrySerialUSART(USARTSerial *serial) : _serial(serial) { }

TelemetrySerialUSART::~TelemetrySerialUSART() { }

void TelemetrySerialUSART::begin(unsigned long baud, uint32_t flags) {
	_serial->begin(baud, flags);
}

int TelemetrySerialUSART::available() {
	return _serial->available();
}

int TelemetrySerialUSART::read() {
	return _serial->read();
}

size_t TelemetrySerialUSART::readBytes(char* buffer, size_t length) {
	return _serial->readBytes(buffer, length);
}
