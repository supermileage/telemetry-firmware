#include "CanControllerMock.h"

CanControllerMock::CanControllerMock(byte msgAvail) : CanController(msgAvail) {
	// set up default functions
	_readInterruptPin = []() { return true; };
	_checkReceive = [this]() { return ~messageAvail(); };
	_getCanId = []() { return 0; };
	_readMsgBuf = [](byte* len, byte* buf) { *len = 1; buf[0] = 0; return 0; };
	_sendMsgBuf = [](unsigned long id, byte ext, byte len, const byte *buf) { };
}

CanControllerMock::~CanControllerMock() { }

bool CanControllerMock::readInterruptPin() { 
	return _readInterruptPin();
}

byte CanControllerMock::checkReceive() {
	if (!_checkReceiveCalled) {
		_checkReceiveCalled = true;
		return _checkReceive();
	} else {
		_checkReceiveCalled = false;
		return ~_checkReceive();
	}
}

byte CanControllerMock::readMsgBuf(byte* len, byte* buf) { 
	return _readMsgBuf(len, buf);
}

unsigned long CanControllerMock::getCanId() { 
	return _getCanId();
}

void CanControllerMock::begin() { }

void CanControllerMock::sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf) {
	_sendMsgBuf(id, ext, len, buf);
}

void CanControllerMock::setReadInterruptPin(std::function<bool(void)> func) {
	_readInterruptPin = func;
}

void CanControllerMock::setCheckReceive(std::function<byte(void)> func) {
	_checkReceive = func;
	_checkReceiveCalled = false;
}

void CanControllerMock::setGetCanId(std::function<uint64_t(void)> func) {
	_getCanId = func;
}

void CanControllerMock::setReadMsgBuffer(std::function<byte(byte*,byte*)> func) {
	_readMsgBuf = func;
}

void CanControllerMock::setSendMsgBuffer(std::function<void(uint64_t,byte,byte,const byte*)> func) {
	_sendMsgBuf = func;
}

void CanControllerMock::setCanMessage(CanMessage msg) {
	setReadInterruptPin([]() { return false; });
	setCheckReceive([this]() { return messageAvail(); });
	setGetCanId([msg]() { return msg.id; });
	setReadMsgBuffer([msg](byte* len, byte* buf) -> byte {
		*len = msg.dataLength;

		for (uint8_t i = 0; i < msg.dataLength; i++)
			buf[i] = msg.data[i];

		return *len;
	});
}

