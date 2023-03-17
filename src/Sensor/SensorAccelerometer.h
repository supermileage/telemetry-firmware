#ifndef _SENSOR_ACCELEROMETER_H_
#define _SENSOR_ACCELEROMETER_H_

#include "settings.h"
#include "Sensor.h"
#include "AccelerometerController.h"

/**
 * @brief directional flags for accelerometer
 * 
 * If you are unsure of which flags to choose, please refer to the telemetry accelerometer
 * documentation on notion: 
*/
#define ACCEL_POSITIVE_X 0x20
#define ACCEL_NEGATIVE_X 0x10
#define ACCEL_POSITIVE_Y 0x8
#define ACCEL_NEGATIVE_Y 0x4
#define ACCEL_POSITIVE_Z 0x2
#define ACCEL_NEGATIVE_Z 0x1

#define GRAVITY 9.81

/*
 * Defines a vehicle accelerometer for which z is forward/backward and y-axis is up/down
 */
class SensorAccelerometer : public Sensor {
    public:
        /**
         * @brief instantiates SensorAccelerometer with same coordinate system as accelerometer
         * @note do not use this constructor unless you are testing, or you know for certain
         * that the vehicle's coordinate system is the same as the accelerometer's
        */
        SensorAccelerometer(AccelerometerController *controller);

        /**
         * @brief instantiates SensorAccelerometer with coordinate system defined direction flags
         * @param controller Accelerometer controller class
         * @param forward the forward direction of the vehicle with respect to the accelerometer's coordinate system
         * @param up the up direction of the vehicle with respect to the accelerometer's coordinate system
        */
        SensorAccelerometer(AccelerometerController *controller, uint16_t forward, uint16_t up);
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
        Matrix3d _transformationMatrix;
        Vec3 _accel;
        Vec3 _gyro;

        int32_t _pitch = 0;
        float _gravityY = 0;
        float _gravityZ = 0;
        bool _initialized = false;

        void _setTransformationMatrix(uint16_t orientation);
        void _setGravityY();
        void _setGravityZ();
        void _setPitch();
        bool _tryRecalibrateGyroscope();
};

#endif