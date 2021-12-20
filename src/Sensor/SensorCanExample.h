#ifndef _SENSOR_CAN_EXAMPLE_H_
#define _SENSOR_CAN_EXAMPLE_H_

#include "Sensor.h"
#include "CanInterface.h"
#include "can_common.h"

#define EXAMPLE_CAN_ID  0x14

class SensorCanExample : public Sensor {

    public:

        /**
         * Constructor 
         * 
         * @param &can object to use
         **/
        SensorCanExample(CanInterface &canInterface) : _canInterface(canInterface) {}

        /**
         * 
         **/
        void begin() {
            _canInterface.addMessageListen(EXAMPLE_CAN_ID);
        }

        /**
         * Dummy Implementation
         **/
        void handle() {}

        String getHumanName() {
            return "CAN Example";
        }

        /**
         * Return some example data for the purposes of demonstration
         **/
        String getData() {
            CanInterface::CanMessage msg = _canInterface.getMessage(EXAMPLE_CAN_ID);
            return String(msg.data[0]);
        }

    private:
        CanInterface &_canInterface;

};

#endif
