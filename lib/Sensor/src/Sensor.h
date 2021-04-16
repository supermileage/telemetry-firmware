#ifndef _SENSOR_H_
#define _SENSOR_H_

class Sensor {
    public:
        virtual void begin() = 0;
        virtual void handle() = 0;

        virtual String getHumanName() = 0;

        /**
         * Returns the time it took to run this function
         * in microseconds.
         */
        long benchmarkedHandle() {
            long start = micros();
            this->handle();
            return micros() - start;
        }
};

#endif
