#include "Mcp2515CanWrapper.h"
#include "can_common.h"

Mcp2515CanWrapper::Mcp2515CanWrapper(SPIClass* spi, uint8_t csPin, uint8_t intPin) : CanController(CAN_MSGAVAIL), _intPin(intPin) {
	_mcpCan = new mcp2515_can(csPin);
	_mcpCan->setSPI(spi);
}

Mcp2515CanWrapper::~Mcp2515CanWrapper() { }

void Mcp2515CanWrapper::begin() {
	pinMode(_intPin, INPUT);
	_mcpCan->begin(CAN_500KBPS,MCP_8MHz);
}

bool Mcp2515CanWrapper::readInterruptPin() {
	return digitalRead(_intPin);
}

byte Mcp2515CanWrapper::checkReceive() {
	return _mcpCan->checkReceive();
}

byte Mcp2515CanWrapper::readMsgBuf(byte* len, byte* buf) {
	return _mcpCan->readMsgBuf(len, buf);
}

unsigned long Mcp2515CanWrapper::getCanId() {
	return _mcpCan->getCanId();
}

void Mcp2515CanWrapper::sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf) {
	_mcpCan->sendMsgBuf(id, ext, len, buf);
}