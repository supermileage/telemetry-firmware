#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#include <vector>

#include "Sensor.h"
#include "mcp2515_can.h"
#include "can_common.h"


class CanInterface : public Handleable {
    public:

        // This struct contains all the components of a CAN message. dataLength must be <= 8, 
        // and the first [dataLength] positions of data[] must contain valid data
        struct CanMessage {
            uint16_t id;
            uint8_t dataLength;
            uint8_t data[8];
        };

        const CanMessage _nullMessage = {0x0, 0, {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

        /**
         * Constructor 
         * @param *spi bus to use for this CAN module
         * @param csPin chip select pin to use for this CAN module
         * @param intPin interrupt pin to use for this CAN module
         **/
        CanInterface(SPIClass *spi, uint8_t csPin, uint8_t intPin);

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
         * @return reference to vector of CAN IDs and their latest data
         **/
        std::vector<CanMessage>& getMessages();

        /**
         * @param id of the desired CAN message
         * 
         * @return the CAN message corresponding to the ID, message of length 0 if ID not found
         **/
        CanMessage getMessage(uint16_t id);

        /**
         * @param id to listen for on CAN bus
         **/
        void addMessageListen(uint16_t id);

    private:
        std::vector<CanMessage> _messages;

        uint8_t _intPin;
        mcp2515_can* _CAN;

};

#endif
