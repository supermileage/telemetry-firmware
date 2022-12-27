#ifndef _SENSOR_ACCELEROMETER_H_
#define _SENSOR_ACCELEROMETER_H_

#include "settings.h"
#include "Sensor.h"
#include "AccelerometerController.h"

class SensorAccelerometer : public Sensor {
    public:
        SensorAccelerometer(AccelerometerController *controller);
        ~SensorAccelerometer();
        String getHumanName() override;
        void begin() override;
        void handle() override;

        Vec3 getGyro();
        Vec3 getAccel();
        float getTemp();
        
        float getAccelMagnitude();
        float getMetersPerSecond();
        float getPitch();
        float getGravityZ();

        String getInitStatus();

    private:
        AccelerometerController *_controller;
        uint64_t _lastOutputMillis = 0;
        float _metersPerSecond = 0;
        int32_t _pitch = 0;
        float _roll = 0;
        float _gravityZ = 0;
        uint64_t _lastTimeMicros = 0;
        bool _initialized = false;
};

#endif