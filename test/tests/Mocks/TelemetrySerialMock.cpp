#include "TelemetrySerialMock.h"
#include <string.h>

TelemetrySerialMock::TelemetrySerialMock() {
	_begin = [](unsigned long baud, uint32_t flags) { };
	_available = []() { return 0; };
	_read = []() { return 0; };
	_readBytes = [](char* buf, size_t len) { return 0; };
}

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

void TelemetrySerialMock::setReadMessage(const uint8_t* msg, int len) {
	_availableCalled = false;

	_available = [this, len]() {
		if (_availableCalled) {
			_availableCalled = false;
			return 0;
		} else {
			_availableCalled = true;
		}

		return len;
	};
	_readBytes = [msg](char* buffer, size_t length) {
		memcpy((void*)buffer, (void*)msg, length);
		return length;
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
