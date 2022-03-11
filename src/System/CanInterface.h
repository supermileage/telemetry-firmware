#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#define ACC_STATUS_HEADLIGHTS		0x0
#define ACC_STATUS_BRAKELIGHTS		0x1
#define ACC_STATUS_HORN				0x2
#define ACC_STATUS_HAZARDS			0x3
#define ACC_STATUS_RIGHT_SIGNAL		0x4
#define ACC_STATUS_LEFT_SIGNAL		0x5
#define ACC_STATUS_WIPERS			0x6
#define ACC_STATUS_NULL				0xFF

#include <map>

#include "can.h"
#include "Sensor.h"
#include "CanBus.h"
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
        CanInterface(CanBus* can);

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
         * @param delegate delegate command which allows can listeners to specify additional parsing behavior
         **/
        void addMessageListen(uint16_t id, Command* canListenerDelegate);
         
        /**
         * @brief Wrapper for sending CAN messages
         * 
         * @param message has the id, data length and data of the message that needs to be sent
         **/
        void sendMessage(CanMessage message);

    private:
        CanBus* _CAN;
        std::map<uint16_t, Command*> _delegates;

};

#endif