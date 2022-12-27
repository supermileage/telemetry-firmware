#include "SensorAccelerometer.h"
#include <math.h>

// #define DEBUG_ACCELEROMETER_OUTPUT_GYRO

#define GRAVITY     9.81
#define X_G_HIGH    9.9f
#define X_G_LOW     -9.72f
#define Y_G_HIGH    9.91f
#define Y_G_LOW     -9.71f
#define Z_G_HIGH    10.15f
#define Z_G_LOW     -9.47f

#define MEGA        1000000

#define OFFSET_X    (X_G_HIGH - GRAVITY)
#define OFFSET_Y    (Y_G_HIGH - GRAVITY)
#define OFFSET_Z    (Z_G_HIGH - GRAVITY)

SensorAccelerometer::SensorAccelerometer(AccelerometerController* controller) : _controller(controller) { }

SensorAccelerometer::~SensorAccelerometer() { }

String SensorAccelerometer::getHumanName() {
    return "Accelerometer";
}

void SensorAccelerometer::begin() {
    _initialized = _controller->init();

    // get initial reading of pitch and ambient gravitational force on z-axis
    if (_initialized) {
        if (_controller->tryGetReading()) {
            Vec3 accel = _controller->getAccel();
            _pitch = acos((-accel.y) / (sqrt(pow(accel.y, 2) + pow(accel.z, 2)))) *
            (accel.z <= 0 ? -1 : 1);
            _gravityZ = sin(_pitch);
            _lastTimeMicros = micros();
        }
    }
}

void SensorAccelerometer::handle() {
    bool success = false;

    #ifdef DEBUG_ACCELEROMETER_OUTPUT_ACCEL
    if (_lastOutputMillis + 15 < millis()) {
        _lastOutputMillis = millis();
        float lastX = _accel.acceleration.x;
        float lastY = _accel.acceleration.y;
        float lastZ = _accel.acceleration.z;
        _lsm6->getEvent(&_accel, &_gyro, &_temp);
        _lastOutputMillis = millis();

        if (fabs(lastX - _accel.acceleration.x) > 0.1 ||
            fabs(lastY - _accel.acceleration.y) > 0.1 ||
            fabs(lastZ - _accel.acceleration.z) > 0.1) {
                DEBUG_SERIAL_LN("< " + String(_accel.acceleration.x - OFFSET_X) + ", " +
                    String(_accel.acceleration.y - OFFSET_Y) + ", " +
                    String(_accel.acceleration.z - OFFSET_Z) + " >");
        }
    }

    #elif defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO)
    if (_lastOutputMillis + 15 < millis()) {
        _lastOutputMillis = millis();
        float lastX = _gyro.gyro.x;
        float lastY = _gyro.gyro.y;
        float lastZ = _gyro.gyro.z;
        _lsm6->getEvent(&_accel, &_gyro, &_temp);

        if (fabs(lastX - _gyro.gyro.x) > 0.1 ||
            fabs(lastY - _gyro.gyro.y) > 0.1 ||
            fabs(lastZ - _gyro.gyro.z) > 0.1) {
                DEBUG_SERIAL_LN("< " + String(_gyro.gyro.x) + ", " +
                    String(_gyro.gyro.y) + ", " +
                    String(_gyro.gyro.z) + " >");
        }
    }
    #endif

    #if !defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO) and !defined(DEBUG_ACCELEROMETER_OUTPUT_ACCEL)
    if (_lastOutputMillis + 15 < millis()) {
        _lastOutputMillis = millis();
        success = _controller->tryGetReading();
    }
    #endif

    // check change in rotation from gyroscope and update pitch
    // use this to update ambient gravitational force on z-axis
    if (success) {
        uint64_t currentTime = micros();
        uint64_t deltaT = currentTime - _lastTimeMicros;
        _lastTimeMicros = currentTime;

        float pitchVelocity = _controller->getGyro().x;
        _pitch = (pitchVelocity * deltaT) + _pitch;
    }
}

Vec3 SensorAccelerometer::getGyro() {
    return _controller->getGyro();
}

Vec3 SensorAccelerometer::getAccel() {
    return _controller->getAccel();
}

float SensorAccelerometer::getTemp() {
    return _controller->getTemp();
}

float SensorAccelerometer::getPitch() {
    return (float)_pitch / MEGA;
}

float SensorAccelerometer::getGravityZ() {
    return _gravityZ;
}

float SensorAccelerometer::getAccelMagnitude() {
    return sqrt(pow(getAccel().x, 2) + pow(getAccel().y, 2) + pow(getAccel().z, 2));
}

float SensorAccelerometer::getMetersPerSecond() {
    return _metersPerSecond;
}

String SensorAccelerometer::getInitStatus() {
    if (_initialized) {
        return "Success -- Offset: " +  String(OFFSET_X) + " " + String(OFFSET_Y) + " " +  String(OFFSET_Z);
    } else {
        return "Failure";
    }
}
