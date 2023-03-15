#ifndef _SENSOR_ACCELEROMETER_H_
#define _SENSOR_ACCELEROMETER_H_

#include "settings.h"
#include "Sensor.h"
#include "AccelerometerController.h"

#define GRAVITY 9.81



/*
 * Defines a vehicle accelerometer sensor positioned such that z-axis
 * is forward/backward and y-axis is up/down
 */
class SensorAccelerometer : public Sensor {
    public:
        SensorAccelerometer(AccelerometerController *controller);
        ~SensorAccelerometer();
        String getHumanName() override;
        void begin() override;
        void handle() override;
        String getHorizontalAcceleration(bool& valid = Sensor::dummy);
        String getVerticalAcceleration(bool& valid = Sensor::dummy);
        String getIncline(bool& valid = Sensor::dummy);

        Vec3 getGyro();
        Vec3 getAccel();
        float getAccelMagnitude();
        String getInitStatus();

    private:
        AccelerometerController *_controller;
        uint64_t _lastReadMillis = 0;
        uint64_t _lastPitchUpdateMicros = 0;

        int32_t _pitch = 0;
        float _gravityY = 0;
        float _gravityZ = 0;
        bool _initialized = false;

        void _setGravityY();
        void _setGravityZ();
        void _setPitch(Vec3 accel);
        bool _tryRecalibrateGyroscope();
};

#endif