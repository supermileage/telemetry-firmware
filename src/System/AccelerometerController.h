#ifndef _ACCELEROMETER_CONTROLLER_H_
#define _ACCELEROMETER_CONTROLLER_H_

struct Vec3 {
    float x;
    float y;
    float z;
};

class AccelerometerController {
    public:
        virtual ~AccelerometerController() { }
        virtual bool init() = 0;
        virtual bool tryGetReading() = 0;
        virtual Vec3 getAccel() = 0;
        virtual Vec3 getGyro() = 0;
        virtual float getTemp() = 0;

};

#endif