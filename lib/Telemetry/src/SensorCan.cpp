#include "SensorCan.h"

/**
 * Constructor 
 * @param *spi bus to use for this CAN module
 * @param csPin chip select pin to use for this CAN module
 * @param intPin interrupt pin to use for this CAN module
 **/
SensorCan::SensorCan(SPIClass *spi, uint8_t csPin, uint8_t intPin) {
    pinMode(intPin, INPUT);
    _intPin = intPin;
    _CAN = new mcp2515_can(csPin);
    _CAN->setSPI(spi);
}

String SensorCan::getHumanName() {
    return "CAN";
}

/**
 * Begin the CAN sensor by setting baud rate and chip freq
 **/
void SensorCan::begin() {
    _CAN->begin(CAN_125KBPS,MCP_8MHz);
}

/**
 * Checks interrupt pin and then checks to make sure a message has been fully received
 * Saves any messages that match the CAN IDs specificed in IDS array
 **/
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

/**
 * @return number of CAN IDs being monitored
 **/
uint8_t SensorCan::getNumIds() {
    return NUM_IDS;
}

/**
 * @param id_num of CAN ID to return
 * @return CAN ID
 **/
uint16_t SensorCan::getId(uint8_t id_num) {
    return IDS[id_num];
}

/**
 * @param id_num of CAN ID to return
 * @return Number of bytes received at this ID
 **/
uint8_t SensorCan::getDataLen(uint8_t id_num) {
    return _dataLen[id_num];
}

/**
 * @param id_num of CAN ID to return
 * @return Pointer to byte array of data received at this ID
 **/
uint8_t* SensorCan::getData(uint8_t id_num) {
    return _data[id_num];
}