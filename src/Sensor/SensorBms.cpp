#include "SensorBms.h"

#define TEMP_ID_INTERNAL            0x00
#define TEMP_ID_BATTERY_1           0x01
#define TEMP_ID_BATTERY_2           0x02

#define REQ_DATA_LENGTH             8

SensorBms::SensorBms(CanInterface &canInterface, uint16_t requestIntervalMs) 
    : CanListener(canInterface, CAN_BMS_RESPONSE), _requestIntervalMs(requestIntervalMs) {}

void SensorBms::handle() {
    if(millis() - _lastValidTime >= _requestIntervalMs) {

        for(int i = 0; i < NUM_PARAMS; i++) {
            CanMessage msg = CAN_MESSAGE_NULL;
            msg.id = CAN_BMS_REQUEST;
            msg.dataLength = REQ_DATA_LENGTH;
            msg.data[0] = _paramIds[i];
            
            _canInterface.sendMessage(msg);
        }
        
        _lastValidTime = millis();
    }
}

String SensorBms::getHumanName() {
    return "BMS";
}

void SensorBms::update(CanMessage message) {
    DEBUG_SERIAL_LN("RECEIVED RESPONSE FROM BMS");
}

String SensorBms::getSoc() {
    return "0.0"; 
}

String SensorBms::getBatteryVolt() {
    return "0.0";
}

String SensorBms::getBatteryCurrent() {
    return "0.0";
}

String SensorBms::getMaxVolt() {
    return "0.0";
}

String SensorBms::getMinVolt() {
    return "0.0";
}

String SensorBms::getStatusBMS() {
    return "0.0";
}

int SensorBms::getTempBMS() {
    return 0;
}

int SensorBms::getBatteryTemp1() {
    return 0;
}

int SensorBms::getBatteryTemp2() {
    return 0;
}