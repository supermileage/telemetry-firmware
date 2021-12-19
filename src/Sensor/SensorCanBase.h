#ifndef _SENSOR_CAN_EXAMPLE_H_
#define _SENSOR_CAN_EXAMPLE_H_

#include "Sensor.h"
#include "CanInterface.h"
#include "can_common.h"

class SensorCanBase : public Sensor {

    public:
        /**
         * Constructor 
         * 
         * @param &can object to use
         **/
        SensorCanBase(CanInterface &canInterface, uint16_t id) : _canInterface(canInterface), _id(id) {}

        virtual void begin() = 0;

        virtual void handle() = 0;

        virtual String getHumanName() = 0;

        /**
         * Returns data (can be overridden in derived classes if more specific behavior is needed)
         **/
        virtual String getData() {
            CanInterface::CanMessage msg = _canInterface.getMessage(_id);
            return String(msg.data[0]);
        }

    protected:
        CanInterface &_canInterface;
        uint16_t _id;

};

#endif
