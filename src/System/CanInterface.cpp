#include "CanInterface.h"

CanInterface::CanInterface(SPIClass *spi, uint8_t csPin, uint8_t intPin) {
    pinMode(intPin, INPUT);
    _intPin = intPin;
    _CAN = new mcp2515_can(csPin);
    _CAN->setSPI(spi);
}

CanInterface::~CanInterface() {
    for (auto const& pair : _messageParsers) {
        delete pair.second;
    }
    delete _CAN;
}

void CanInterface::begin() {
    _CAN->begin(CAN_500KBPS,MCP_8MHz);
}

void CanInterface::handle() {
    if(!digitalRead(_intPin)){
        while(_CAN->checkReceive() == CAN_MSGAVAIL){
            uint8_t len = 0;
            uint8_t data[8];
            _CAN->readMsgBuf(&len, data);
            unsigned long canId = _CAN->getCanId();

            // check if we're listening for canId, continue if we're not
            if(_messages.find(canId) == _messages.end()){
                continue;
            }

            CanMessage& message = _messages[canId];
            message.dataLength = len;
            for(int i = 0; i < len; i++){
                message.data[i] = data[i];
            }

            if (_messageParsers[canId]) {
                _messageParsers[canId]->execute((void*)&message);
            }
        }
    }
}

void CanInterface::addMessageListen(uint16_t id, Delegate* delegate) {
    CanMessage newMessage = _nullMessage;
    newMessage.id = id;
    _messages[id] = newMessage;
    _messageParsers[id] = delegate;
}

std::map<uint16_t, CanInterface::CanMessage>& CanInterface::getMessages(){
    return _messages;
}

CanInterface::CanMessage CanInterface::getMessage(uint16_t id) {
    return _messages[id];
}