#ifndef _SENSOR_H_
#define _SENSOR_H_

class Sensor {
    public:
        virtual void begin() = 0;
        virtual void handle() = 0;
};

#endif
