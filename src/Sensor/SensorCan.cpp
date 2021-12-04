#include "SensorCan.h"

SensorCan::SensorCan(SPIClass *spi, uint8_t csPin, uint8_t intPin) {
    pinMode(intPin, INPUT);
    _intPin = intPin;
    _CAN = new mcp2515_can(csPin);
    _CAN->setSPI(spi);
}

String SensorCan::getHumanName() {
    return "CAN";
}

void SensorCan::begin() {
    _CAN->begin(CAN_500KBPS,MCP_8MHz);
}

void SensorCan::handle() {
    if(!digitalRead(_intPin)){
        while(_CAN->checkReceive() == CAN_MSGAVAIL){
            uint8_t len = 0;
            uint8_t data[8];
            _CAN->readMsgBuf(&len, data);
            unsigned long canId = _CAN->getCanId();

            for(CanMessage& m : _messages){
                if(canId == m.id){
                    m.dataLength = len;
                    for(int i = 0; i < len; i++){
                        m.data[i] = data[i];
                    }
                    return;
                }
            }
            
        }
    }
}

std::vector<SensorCan::CanMessage>& SensorCan::getMessages(){
    return _messages;
}

SensorCan::CanMessage SensorCan::getMessage(uint16_t id) {
    for(CanMessage m : _messages){
        if(id == m.id){
            return m;
        }
    }

    return _nullMessage;
}

void SensorCan::addMessageListen(uint16_t id) {
    CanMessage newMessage = _nullMessage;
    newMessage.id = id;
    _messages.push_back(newMessage);
}
