#include "TelemetrySerialMock.h"

TelemetrySerialMock::~TelemetrySerialMock() { }

void TelemetrySerialMock::begin(unsigned long baud, uint32_t flags) {
	_begin(baud, flags);
}

int TelemetrySerialMock::available() {
	return _available();
}

int TelemetrySerialMock::read() {
	return _read();
}

size_t TelemetrySerialMock::readBytes(char* buffer, size_t length) {
	return _readBytes(buffer, length);
}

void TelemetrySerialMock::setReadMessage(std::string message) {
	int length = message.length();
	_available = [length]() -> int { return length; };
	_readBytes = [message](char* buffer, size_t length) -> size_t {
		strcpy(buffer, message.c_str());
		return strlen(buffer);
	};
}

void TelemetrySerialMock::setBegin(std::function<void(unsigned long, uint32_t)> func) {
	_begin = func;
}

void TelemetrySerialMock::setAvailable(std::function<int(void)> func) {
	_available = func;
}

void TelemetrySerialMock::setRead(std::function<int(void)> func) {
	_read = func;
}

void TelemetrySerialMock::setReadBytes(std::function<size_t(char*,size_t)> func) {
	_readBytes = func;
}
