#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "Particle.h"
#include "Handleable.h"

// Interval (in ms) after which telemetry will consider data to be invalid
#define STALE_INTERVAL          2000

class Sensor : public Handleable {
    public:
        /**
         * @brief represents an on-off status for a sensor device
         */
        enum Status { Off = 0, On = 1, Unknown = 2 };
	
        /**
         * INTERFACE
         * @return Human name for this sensor type
         * */
        virtual String getHumanName() = 0;

    protected:
        static bool dummy;
};

#endif