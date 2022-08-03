#include "CanBusMock.h"

CanBusMock::CanBusMock(byte messageAvail) : CanBus(messageAvail) {
	// set up default functions
	_readInterruptPin = []() { return true; };
	_checkReceive = []() { return 0; };
	_getCanId = []() { return 0; };
	_readMsgBuf = [](byte* len, byte* buf) { *len = 1; buf[0] = 0; return 0; };
	_sendMsgBuf = [](unsigned long id, byte ext, byte len, const byte *buf) { };
}

CanBusMock::~CanBusMock() { }

bool CanBusMock::readInterruptPin() { 
	return _readInterruptPin();
}

byte CanBusMock::checkReceive() {
	if (!_checkReceiveCalled) {
		_checkReceiveCalled = true;
		return _checkReceive();
	} else {
		return ~_checkReceive();
	}
}

byte CanBusMock::readMsgBuf(byte* len, byte* buf) { 
	return _readMsgBuf(len, buf);
}

unsigned long CanBusMock::getCanId() { 
	return _getCanId();
}

void CanBusMock::begin() { }

void CanBusMock::sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf) {
	_sendMsgBuf(id, ext, len, buf);
}

void CanBusMock::setReadInterruptPin(std::function<bool(void)> func) {
	_readInterruptPin = func;
}

void CanBusMock::setCheckReceive(std::function<byte(void)> func) {
	_checkReceive = func;
	_checkReceiveCalled = false;
}

void CanBusMock::setGetCanId(std::function<uint64_t(void)> func) {
	_getCanId = func;
}

void CanBusMock::setReadMsgBuffer(std::function<byte(byte*,byte*)> func) {
	_readMsgBuf = func;
}

void CanBusMock::setSendMsgBuffer(std::function<void(uint64_t,byte,byte,const byte*)> func) {
	_sendMsgBuf = func;
}

