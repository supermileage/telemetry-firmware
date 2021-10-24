#ifndef _SENSOR_CAN_H_
#define _SENSOR_CAN_H_

#include "Sensor.h"
#include "mcp2515_can.h"
#include "can_common.h"

// Number of CAN IDs to listen for
#define NUM_IDS 2

class SensorCan : public Sensor {
    public:

        /**
         * Constructor 
         * @param *spi bus to use for this CAN module
         * @param csPin chip select pin to use for this CAN module
         * @param intPin interrupt pin to use for this CAN module
         **/
        SensorCan(SPIClass *spi, uint8_t csPin, uint8_t intPin);

        /**
         * Begin the CAN sensor by setting baud rate and chip freq
         **/
        void begin();
        /**
         * Checks interrupt pin and then checks to make sure a message has been fully received
         * Saves any messages that match the CAN IDs specificed in IDS array
         **/
        void handle();
        String getHumanName();

        /**
         * @return number of CAN IDs being monitored
         **/
        uint8_t getNumIds();

        /**
         * @param id_num of CAN ID to return
         * @return CAN ID
         **/
        uint16_t getId(uint8_t id_num);

        /**
         * @param id_num of CAN ID to return
         * @return Number of bytes received at this ID
         **/
        uint8_t getDataLen(uint8_t id_num);

        /**
         * @param id_num of CAN ID to return
         * @return Pointer to byte array of data received at this ID
         **/
        uint8_t* getData(uint8_t id_num);

    private:
        // CAN IDs to listen for
        const uint16_t IDS[NUM_IDS] = {CAN_THROTTLE, CAN_MOTOR_ENABLE};

        uint8_t _dataLen[NUM_IDS];
        uint8_t _data[NUM_IDS][8];

        uint8_t _intPin;
        mcp2515_can* _CAN;

};

#endif
