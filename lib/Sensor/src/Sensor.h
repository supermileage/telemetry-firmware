#include "Particle.h"

#ifndef _SENSOR_H_
#define _SENSOR_H_

class Sensor {
    public:

        /**
         * INTERFACE
         * Initialize sensor
         * */
        virtual void begin() = 0;

        /**
         * INTERFACE
         * Sensor update handler, run as frequently as possible
         * */
        virtual void handle() = 0;

        /**
         * INTERFACE
         * @return Human name for this sensor type
         * */
        virtual String getHumanName() = 0;

        /**
         * Run handle function of sensor and record time elapsed if it's longer than longest time elapsed
         * */
        void benchmarkedHandle();

        /**
         * @return longest handle time since last checked
         * */
        long getLongestHandleTime();

    private:
        long _longestHandleTime = 0;
};

#endif
