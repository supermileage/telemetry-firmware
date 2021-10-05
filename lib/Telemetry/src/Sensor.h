#include "Particle.h"

#ifndef _SENSOR_H_
#define _SENSOR_H_

class Sensor {
    public:
        virtual void begin() = 0;
        virtual void handle() = 0;

        virtual String getHumanName() = 0;

        void benchmarkedHandle();
        long getLongestHandleTime();

    private:
        long _longestHandleTime = 0;
};

#endif
