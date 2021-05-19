#ifndef _SENSOR_CAN_H_
#define _SENSOR_CAN_H_

#include "Sensor.h"
#include "mcp2515_can.h"
#include "can_common.h"

#define NUM_IDS 2

class SensorCan : public Sensor {
    public:
        SensorCan(SPIClass *spi, uint8_t csPin, uint8_t intPin);

        void begin();
        void handle();
        String getHumanName();

        uint8_t getNumIds();
        uint16_t getId(uint8_t id_num);
        uint8_t getDataLen(uint8_t id_num);
        uint8_t* getData(uint8_t id_num);

    private:
        const uint16_t IDS[NUM_IDS] = {CAN_THROTTLE, CAN_MOTOR_ENABLE};
        uint8_t _dataLen[NUM_IDS];
        uint8_t _data[NUM_IDS][8];

        uint8_t _intPin;
        mcp2515_can* _CAN;

};

#endif
