#ifndef _SENSOR_ACCELEROMETER_H_
#define _SENSOR_ACCELEROMETER_H_

#include "settings.h"
#include "Sensor.h"
#include "AccelerometerController.h"

#define GRAVITY 9.81

class SensorAccelerometer : public Sensor {
    public:
        SensorAccelerometer(AccelerometerController *controller);
        ~SensorAccelerometer();
        String getHumanName() override;
        void begin() override;
        void handle() override;

        float getHorizontalAcceleration(bool& valid = Sensor::dummy);
        float getVerticalAcceleration(bool& valid = Sensor::dummy);
        float getPitch(bool& valid = Sensor::dummy);


        Vec3 getGyro();
        Vec3 getAccel();
        float getTemp();
        float getAccelMagnitude();
        float getMetersPerSecond();
        float getGravityZ();

        String getInitStatus();

    private:
        AccelerometerController *_controller;
        uint64_t _lastReadMillis = 0;
        uint64_t _lastPitchUpdateMicros = 0;

        int32_t _pitch = 0;
        float _horizontalAcceleration = 0;
        float _verticalAcceleration = 0;
        float _metersPerSecond = 0;
        float _gravityY = 0;
        float _gravityZ = 0;
        bool _initialized = false;

        void _setGravityY();
        void _setGravityZ();
        void _setPitch(Vec3 accel);
        bool _tryRecalibrateGyroscope();
};

#endif