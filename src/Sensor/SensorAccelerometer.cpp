#include <math.h>
#include "SensorAccelerometer.h"

#define GYRO_RECALIBRATION_MARGIN   0.1f     // if accelerometer |<y,z> - 9.81^2| <= margin, recalibrate gyro readings
#define MEGA                        1000000
#define ALPHA                        0.01f

#define ACCEL_FORWARD_Z_UP_Y        0x208
#define ACCEL_FORWARD_Z_DOWN_Y      0x204
#define ACCEL_BACKWARD_Z_UP_Y       0x108
#define ACCEL_BACKWARD_Z_DOWN_Y     0x104
#define ACCEL_FORWARD_X_UP_Z        0x2002
#define ACCEL_FORWARD_X_DOWN_Z      0x2001
#define ACCEL_FORWARD_Y_UP_X        0x820
#define ACCEL_BACKWARD_Y_DOWN_Z     0x401
#define ACCEL_FORWARD_Z_UP_X        0x220
#define ACCEL_FORWARD_Y_DOWN_Z      0x801

/* currently unused */
#define ACCEL_FORWARD_Z_UP_X        0x220
#define ACCEL_FORWARD_Z_DOWN_X      0x210
#define ACCEL_BACKWARD_Z_UP_X       0x120
#define ACCEL_BACKWARD_Z_DOWN_X     0x110
#define ACCEL_FORWARD_Y_DOWN_X      0x810
#define ACCEL_FORWARD_Y_UP_Z        0x802
#define ACCEL_BACKWARD_Y_UP_X       0x420
#define ACCEL_BACKWARD_Y_DOWN_X     0x410
#define ACCEL_BACKWARD_Y_UP_Z       0x402
#define ACCEL_FORWARD_X_UP_Y        0x2008
#define ACCEL_FORWARD_X_DOWN_Y      0x2004
#define ACCEL_BACKWARD_X_UP_Y       0x1008
#define ACCEL_BACKWARD_X_DOWN_Y     0x1004
#define ACCEL_BACKWARD_X_UP_Z       0x1002
#define ACCEL_BACKWARD_X_DOWN_Z     0x1001

/* Debug Settings */
// #define DEBUG_ACCELEROMETER_OUTPUT_GYRO
// #define DEBUG_ACCELEROMETER_OUTPUT_ACCEL

#if defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO) or defined(DEBUG_ACCELEROMETER_OUTPUT_ACCEL)
int displayCount = 0;
#endif

SensorAccelerometer::SensorAccelerometer(AccelerometerController* controller, uint32_t interval) : _controller(controller) {
    _setTransformationMatrix(ACCEL_FORWARD_Z_UP_Y);
    _samples = std::vector<Vec3>(interval / 9.6f, Vec3 {0.f, 0.f, 0.f});
    _sampleScale = 1.f / _samples.size();
}

SensorAccelerometer::SensorAccelerometer(AccelerometerController* controller, uint32_t interval, uint16_t forward, uint16_t up) :
    _controller(controller) {
        _setTransformationMatrix((forward << 8) | up);
}

SensorAccelerometer::~SensorAccelerometer() { }

String SensorAccelerometer::getHumanName() {
    return "Accelerometer";
}

void SensorAccelerometer::begin() {
    _initialized = _controller->init();

    // get initial reading of pitch and gravitational acceleration on z and y axes.
    if (_initialized) {
        if (_controller->tryGetReading()) {
            _accel = _transformationMatrix.multiply(_controller->getAccel());
            _gyro = _transformationMatrix.multiply(_controller->getGyro());
            _setPitch();
        }
    }
}

void SensorAccelerometer::handle() {
    bool success = false;

    #ifdef DEBUG_ACCELEROMETER_OUTPUT_ACCEL
    if (_lastReadMillis + ACCEL_READ_INTERVAL < millis()) {
        _lastReadMillis = millis();
        float lastX = _controller->getAccel().x;
        float lastY = _controller->getAccel().y;
        float lastZ = _controller->getAccel().z;
        success = _controller->tryGetReading();
        _lastReadMillis = millis();

        if (fabs(lastX - _controller->getAccel().x) > 0.1 ||
            fabs(lastY - _controller->getAccel().y) > 0.1 ||
            fabs(lastZ - _controller->getAccel().z) > 0.1) {
                if (displayCount++ % 10 == 0) {
                    DEBUG_SERIAL_LN("< " + String(_controller->getAccel().x) + ", " +
                        String(_controller->getAccel().y) + ", " +
                        String(_controller->getAccel().z) + " >");
                }
        }
    }
    #elif defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO)
    if (_lastReadMillis + ACCEL_READ_INTERVAL < millis()) {
        _lastReadMillis = millis();
        float lastX = _controller->getGyro().x;
        float lastY = _controller->getGyro().y;
        float lastZ = _controller->getGyro().z;
        success = _controller->tryGetReading();

        if (fabs(lastX - _controller->getGyro().x) > 0.1 ||
            fabs(lastY - _controller->getGyro().y) > 0.1 ||
            fabs(lastZ - _controller->getGyro().z) > 0.1) {
                if (displayCount++ % 10 == 0) {
                    DEBUG_SERIAL_LN("< " + String(_controller->getGyro().x) + ", " +
                        String(_controller->getGyro().y) + ", " +
                        String(_controller->getGyro().z) + " >");
                }
        }
    }
    #endif

    #if !defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO) and !defined(DEBUG_ACCELEROMETER_OUTPUT_ACCEL)
    if (_lastReadMillis + ACCEL_READ_INTERVAL <= millis()) {
        _lastReadMillis = millis();
        success = _controller->tryGetReading();
    }
    #endif

    if (success) {
        // apply moving average convolutional filter
        _samples[_head] = _transformationMatrix.multiply(_controller->getAccel());
        uint16_t next = (_head + 1) % static_cast<uint16_t>(_samples.size());
        
        Vec3 tail;
        if (_numSamples != _samples.size()) {
            // if vector still isn't fully populated, subtract padding
            ++_numSamples;
            tail = _samples[0];
        } else {
            // otherwise, subtract tail entry
            tail = _samples[next];
        }
        _accel = _accel + (_samples[_head] - tail) * _sampleScale;
        _head = next;

        // gyro is quite accurate without smoothing
        _gyro = _transformationMatrix.multiply(_controller->getGyro());

        if (!_tryRecalibrateGyroscope()) {
            uint64_t currentTime = micros();
            uint64_t deltaT = currentTime - _lastPitchUpdateMicros;
            _lastPitchUpdateMicros = currentTime;
            _pitch = (_gyro.x * deltaT) + _pitch;
        }
    }
}

Vec3 SensorAccelerometer::getGyro() {
    return _gyro;
}

Vec3 SensorAccelerometer::getAccel() {
    return _accel;
}

String SensorAccelerometer::getHorizontalAcceleration(bool& valid) {
    valid = _initialized;
    return FLOAT_TO_STRING(_accel.z, 2);
}

String SensorAccelerometer::getVerticalAcceleration(bool& valid) {
    valid = _initialized;
    return FLOAT_TO_STRING(_accel.y, 2);
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

void SensorAccelerometer::_setPitch() {
    // <1,0> • <y,z> = |<y,z>| cos(t)  ->  t = arccos(y / |<y,z>|)
    _pitch = acos((_accel.y) / (sqrt((_accel.y * _accel.y) + (_accel.z * _accel.z)))) *
        (_accel.z <= 0 ? -1 : 1) * MEGA;

    if (_pitch != 0) {
        _lastPitchUpdateMicros = micros();
        return;    
    }
    _lastPitchUpdateMicros = micros();
}

bool SensorAccelerometer::_tryRecalibrateGyroscope() {
    // if we not reading G on y (car is not level)
    if (fabs((_accel.y * _accel.y) - (ACCEL_GRAVITY * ACCEL_GRAVITY)) >= GYRO_RECALIBRATION_MARGIN) {
        // and if the magnitude of x and y is close to G
        if (fabs((_accel.y * _accel.y + _accel.z * _accel.z) - (ACCEL_GRAVITY * ACCEL_GRAVITY)) <= GYRO_RECALIBRATION_MARGIN) {
            #if defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO) or defined(DEBUG_ACCELEROMETER_OUTPUT_ACCEL)
            DEBUG_SERIAL_LN("RECALIBRATING GYROSCOPE!");
            #endif
            
            // use current y,z readings to set the current pitch
            _setPitch();
            return true;
        }
    }
    return false;
}

/**
 * @note: there are so many configurations to consider that I've only added the ones we
 * currently need.  This can be fleshed out later
*/
void SensorAccelerometer::_setTransformationMatrix(uint16_t orientation) {
    switch (orientation) {
        case ACCEL_FORWARD_Z_UP_Y:
            // identity matrix
            _transformationMatrix.setMatrix({{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}});
            break;
        case ACCEL_FORWARD_Z_DOWN_Y:
            _transformationMatrix.setMatrix({{{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}}});
            break;
        case ACCEL_BACKWARD_Z_UP_Y:
            _transformationMatrix.setMatrix({{{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}}});
            break;
        case ACCEL_BACKWARD_Z_DOWN_Y:
            _transformationMatrix.setMatrix({{{1, 0, 0}, {0, -1, 0}, {0, 0, -1}}});
            break;
        case ACCEL_FORWARD_X_UP_Z:
            _transformationMatrix.setMatrix({{{0, 0, 1}, {0, 0, 1}, {0, 1, 0}}});
            break;
        case ACCEL_FORWARD_X_DOWN_Z:
            _transformationMatrix.setMatrix({{{0, 0, 1}, {0, 0, 1}, {0, -1, 0}}});
            break;
        case ACCEL_FORWARD_Y_UP_Z:
            _transformationMatrix.setMatrix({{{1, 0, 0}, {0, 0, 1}, {0, 1, 0}}});
            break;
        case ACCEL_BACKWARD_Y_DOWN_Z:
            _transformationMatrix.setMatrix({{{1, 0, 0}, {0, 0, -1}, {0, -1, 0}}});
            break;
        case ACCEL_FORWARD_Y_DOWN_Z:
            _transformationMatrix.setMatrix({{{1, 0, 0}, {0, 0, 1}, {0, -1, 0}}});
            break;
        case ACCEL_FORWARD_Z_UP_X:
            _transformationMatrix.setMatrix({{{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}}});
            break;
        default:
            break;
    }
}
