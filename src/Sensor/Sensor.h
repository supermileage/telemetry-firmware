#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "Particle.h"
#include "Handleable.h"

class Sensor : public Handleable {
    public:

        /**
         * Initialize sensor
         * */
        virtual void begin() = 0;

        /**
         * Sensor update handler, run as frequently as possible
         * */
        virtual void handle() = 0;

        /**
         * INTERFACE
         * @return Human name for this sensor type
         * */
        virtual String getHumanName() = 0;

    protected:
    
        static bool dummy;
};

#endif
