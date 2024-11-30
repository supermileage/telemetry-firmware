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
#define ACCEL_POSITIVE_X    0x20
#define ACCEL_NEGATIVE_X    0x10
#define ACCEL_POSITIVE_Y    0x8
#define ACCEL_NEGATIVE_Y    0x4
#define ACCEL_POSITIVE_Z    0x2
#define ACCEL_NEGATIVE_Z    0x1

#define ACCEL_GRAVITY       9.8067F
#define ACCEL_READ_INTERVAL 10   // read interval in ms -- keep in mind that default data rate for LSM6DOX is 104Hz

/**
 * @brief Defines a vehicle accelerometer sensor positioned such that z-axis is forward/backward
 * and y-axis is up/down -- uses running mean convolution to denoise acceleration data
 * 
 * @note extra documentation here: https://www.notion.so/Accelerometer-3aa7452fe9784e0f9762a47d624e0669 
 *
 */
class SensorAccelerometer : public Sensor {
    public:
        /**
         * @brief instantiates SensorAccelerometer with same coordinate system as accelerometer and moving average
         * along interval
         * @note do not use this constructor unless you are testing, or you know for certain
         * that the vehicle's coordinate system is the same as the accelerometer's
        */
        SensorAccelerometer(AccelerometerController *controller, uint32_t interval);

        /**
         * @brief instantiates SensorAccelerometer with coordinate system defined direction flags
         * @param controller Accelerometer controller class
         * @param interval interval along which moving average will be computed
         * @param forward the forward direction of the vehicle with respect to the accelerometer's coordinate system
         * @param up the up direction of the vehicle with respect to the accelerometer's coordinate system
        */
        SensorAccelerometer(AccelerometerController *controller, uint32_t interval, uint16_t forward, uint16_t up);
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
        std::vector<Vec3> _samples;
        uint16_t _head = 0;
        uint16_t _numSamples = 0;
        float _sampleScale;

        uint64_t _lastReadMillis = 0;
        uint64_t _lastPitchUpdateMicros = 0;
        Matrix3d _transformationMatrix;
        Vec3 _accel = Vec3 { 0, 0, 0 };
        Vec3 _gyro = Vec3 { 0, 0, 0 };

        int32_t _pitch = 0;     // rad * 1000
        bool _initialized = false;

        void _setTransformationMatrix(uint16_t orientation);
        void _setPitch();
        bool _tryRecalibrateGyroscope();
};

#endif