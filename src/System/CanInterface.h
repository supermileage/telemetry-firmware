#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#include <map>

#include "can.h"
#include "Sensor.h"
#include "mcp2515_can.h"
#include "can_common.h"
#include "Command.h"

using namespace can;

class CanInterface : public Handleable {
    public:
        /**
         * Constructor 
         * @param *spi bus to use for this CAN module
         * @param csPin chip select pin to use for this CAN module
         * @param intPin interrupt pin to use for this CAN module
         **/
        CanInterface(SPIClass *spi, uint8_t csPin, uint8_t intPin);

        // look into virtual desctructors in c++
        ~CanInterface();

        /**
         * Begin the CAN sensor by setting baud rate and chip freq
         **/
        void begin();

        /**
         * Checks interrupt pin and then checks to make sure a message has been fully received
         * Saves any messages that match the CAN IDs specificed in IDS array
         **/
        void handle();

        /**
         * @brief Adds message id for can interface to listen to
         * 
         * @param id to listen for on CAN bus
         * @param delegate optional delegate which allows can listeners to specify additional parsing behavior
         **/
        void addMessageListen(uint16_t id, Command* canListenerDelegate);

    private:
        std::map<uint16_t, Command*> _delegates;
        uint8_t _intPin;
        mcp2515_can* _CAN;

};

#endif