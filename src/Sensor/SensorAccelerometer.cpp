#include <math.h>
#include "SensorAccelerometer.h"

// #define DEBUG_ACCELEROMETER_OUTPUT_GYRO
// #define DEBUG_ACCELEROMETER_OUTPUT_ACCEL

#define GYRO_RECALIBRATION_MARGIN   0.01    // if accelerometer |<y,z>| == 9.81, recalibrate gyroscope
#define READ_INTERVAL               10      // keep in mind that default data rate for LSM6DOX is 104Hz
#define MEGA                        1000000

SensorAccelerometer::SensorAccelerometer(AccelerometerController* controller) : _controller(controller) { }

SensorAccelerometer::~SensorAccelerometer() { }

String SensorAccelerometer::getHumanName() {
    return "Accelerometer";
}

void SensorAccelerometer::begin() {
    _initialized = _controller->init();

    // assign method-pointers
    
    // get initial reading of pitch and gravitational acceleration on z and y axes.
    // assumes that the vehicle is not moving when this method is called
    if (_initialized) {
        if (_controller->tryGetReading()) {
            _setPitch(_controller->getAccel());
            _setGravityY();
            _setGravityZ();
        }
    }
}

void SensorAccelerometer::handle() {
    bool success = false;

    #ifdef DEBUG_ACCELEROMETER_OUTPUT_ACCEL
    if (_lastReadMillis + READ_INTERVAL < millis()) {
        _lastReadMillis = millis();
        float lastX = _controller.getAccel().x;
        float lastY = _controller.getAccel().y;
        float lastZ = _controller.getAccel().z;
        success = _controller->tryGetReading();
        _lastReadMillis = millis();

        if (fabs(lastX - _controller.getAccel().x) > 0.1 ||
            fabs(lastY - _controller.getAccel().y) > 0.1 ||
            fabs(lastZ - _controller.getAccel().z) > 0.1) {
                DEBUG_SERIAL_LN("< " + String(_controller.getAccel().x - OFFSET_X) + ", " +
                    String(_controller.getAccel().y - OFFSET_Y) + ", " +
                    String(_controller.getAccel().z - OFFSET_Z) + " >");
        }
    }
    #elif defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO)
    if (_lastReadMillis + READ_INTERVAL < millis()) {
        _lastReadMillis = millis();
        float lastX = _controller.getGyro().x;
        float lastY = _controller.getGyro().y;
        float lastZ = _controller.getGyro().z;
        success = _controller->tryGetReading();

        if (fabs(lastX - _controller.getGyro().x) > 0.1 ||
            fabs(lastY - _controller.getGyro().y) > 0.1 ||
            fabs(lastZ - _controller.getGyro().z) > 0.1) {
                DEBUG_SERIAL_LN("< " + String(_controller.getGyro().x) + ", " +
                    String(_controller.getGyro().y) + ", " +
                    String(_controller.getGyro().z) + " >");
        }
    }
    #endif

    #if !defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO) and !defined(DEBUG_ACCELEROMETER_OUTPUT_ACCEL)
    if (_lastReadMillis + READ_INTERVAL < millis()) {
        _lastReadMillis = millis();
        success = _controller->tryGetReading();
    }
    #endif

    if (success) {
        if (!_tryRecalibrateGyroscope()) {
            uint64_t currentTime = micros();
            uint64_t deltaT = currentTime - _lastPitchUpdateMicros;
            _lastPitchUpdateMicros = currentTime;
            _pitch = (_controller->getGyro().x * deltaT) + _pitch;
        }
        
        // update gravitational acceleration on y, z based on new pitch
        _setGravityY();
        _setGravityZ();
    }
}

Vec3 SensorAccelerometer::getGyro() {
    return _controller->getGyro();
}

Vec3 SensorAccelerometer::getAccel() {
    return _controller->getAccel();
}

String SensorAccelerometer::getHorizontalAcceleration(bool& valid) {
    valid = _initialized;
    return FLOAT_TO_STRING(_controller->getAccel().z - _gravityZ, 2);
}

String SensorAccelerometer::getVerticalAcceleration(bool& valid) {
    valid = _initialized;
    return FLOAT_TO_STRING(_controller->getAccel().y - _gravityY, 2);
}

String SensorAccelerometer::getIncline(bool& valid) {
    valid = _initialized;
    return FLOAT_TO_STRING((float)_pitch / MEGA, 2);
}

float SensorAccelerometer::getAccelMagnitude() {
    return sqrt(pow(getAccel().x, 2) + pow(getAccel().y, 2) + pow(getAccel().z, 2));
}

String SensorAccelerometer::getInitStatus() {
    if (_initialized) {
        return "Success";
    } else {
        return "Failure";
    }
}

void SensorAccelerometer::_setGravityY() {
    _gravityY = -(GRAVITY * cos((float)_pitch / MEGA));
}

void SensorAccelerometer::_setGravityZ() {
    _gravityZ = GRAVITY * sin((float)_pitch / MEGA);
}

void SensorAccelerometer::_setPitch(Vec3 accel) {
    // <1,0> • <y,z> = |<y,z>| cos(t)  ->  t = arccos(y / |<y,z>|)
    _pitch = acos((-accel.y) / (sqrt(pow(accel.y, 2) + pow(accel.z, 2)))) *
        (accel.z <= 0 ? -1 : 1) * MEGA;
    _lastPitchUpdateMicros = micros();
}

bool SensorAccelerometer::_tryRecalibrateGyroscope() {
    Vec3 accel = _controller->getAccel();

    if (fabs((accel.z * accel.z + accel.y * accel.y) - (GRAVITY * GRAVITY)) <= GYRO_RECALIBRATION_MARGIN) {
        _setPitch(accel);
        return true;
    }

    return false;
}
