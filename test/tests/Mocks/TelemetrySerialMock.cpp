#include "TelemetrySerialMock.h"
#include <string.h>

#define DEFAULT_TIMEOUT 1000

TelemetrySerialMock::TelemetrySerialMock() {
    _begin = [](unsigned long baud, uint32_t flags) { };
    _available = []() { return 0; };
    _read = [this]() {
        if (_buffer.empty()) return -1;
        uint8_t val = _buffer.front();
        _buffer.erase(_buffer.begin());
        return static_cast<int>(val); // Fix: cast to int
    };
    _peek = [this]() {
        if (_buffer.empty()) return -1;
        return static_cast<int>(_buffer.front()); // Fix: cast to int
    };
    _readBytes = [this](char* buf, size_t len) {
        size_t toRead = std::min(len, _buffer.size());
        for (size_t i = 0; i < toRead; ++i) {
            buf[i] = _buffer[i];
        }
        _buffer.erase(_buffer.begin(), _buffer.begin() + toRead);
        return toRead;
    };
    _peekBytes = [this](char* buf, size_t len) {
        size_t toPeek = std::min(len, _buffer.size());
        for (size_t i = 0; i < toPeek; ++i) {
            buf[i] = _buffer[i];
        }
        return toPeek;
    };
    setTimeout(DEFAULT_TIMEOUT);
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

void TelemetrySerialMock::setTimeout(unsigned long timeout) {
	_timeout = timeout;
}

int TelemetrySerialMock::peek() {
	return _peek();
}

void TelemetrySerialMock::setPeek(std::function<int(void)> func) {
    _peek = func;
}

size_t TelemetrySerialMock::peekBytes(char* buffer, size_t length) {
    return _peekBytes(buffer, length);
}

void TelemetrySerialMock::setBuffer(const std::vector<uint8_t>& data) {
    _buffer = data;
}
