#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#include <unordered_map>

#include "can.h"
#include "Sensor.h"
#include "CanController.h"
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
        CanInterface(CanController* can);

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
        CanController* _CAN;
        std::unordered_map<uint16_t, Command*> _delegates;

};

#endif