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
    _CAN->begin(CAN_125KBPS,MCP_8MHz);
}

void SensorCan::handle() {
    if(!digitalRead(_intPin)){
        if(_CAN->checkReceive() == CAN_MSGAVAIL){
            unsigned char len = 0;
            unsigned char data[8];
            _CAN->readMsgBuf(&len, data);
            unsigned long canId = _CAN->getCanId();

            for(unsigned int i = 0; i < NUM_IDS; i++){
                if(canId == IDS[i]){
                    _dataLen[i] = len;
                    for(int k = 0; k < len; k++){
                        _data[i][k] = data[k];
                    }
                    return;
                }
            }
        }
    }
}

uint8_t SensorCan::getNumIds() {
    return NUM_IDS;
}

uint16_t SensorCan::getId(uint8_t id_num) {
    return IDS[id_num];
}

uint8_t SensorCan::getDataLen(uint8_t id_num) {
    return _dataLen[id_num];
}

uint8_t* SensorCan::getData(uint8_t id_num) {
    return _data[id_num];
}