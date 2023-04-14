#include <math.h>
#include "SensorAccelerometer.h"

#define GYRO_RECALIBRATION_MARGIN   0.1     // if accelerometer |<y,z> - 9.81^2| < margin, recalibrate gyro readings
#define READ_INTERVAL               10      // keep in mind that default data rate for LSM6DOX is 104Hz
#define MEGA                        1000000

#define ACCEL_FORWARD_Z_UP_Y 0x208
#define ACCEL_FORWARD_Z_DOWN_Y 0x204
#define ACCEL_BACKWARD_Z_UP_Y 0x108
#define ACCEL_BACKWARD_Z_DOWN_Y 0x104
#define ACCEL_FORWARD_X_UP_Z 0x2002
#define ACCEL_FORWARD_X_DOWN_Z 0x2001
#define ACCEL_FORWARD_Y_UP_X 0x820
#define ACCEL_BACKWARD_Y_DOWN_Z 0x401
#define ACCEL_FORWARD_Z_UP_X 0x220

/* currently unused */
#define ACCEL_FORWARD_Z_UP_X 0x220
#define ACCEL_FORWARD_Z_DOWN_X 0x210
#define ACCEL_BACKWARD_Z_UP_X 0x120
#define ACCEL_BACKWARD_Z_DOWN_X 0x110
#define ACCEL_FORWARD_Y_DOWN_X 0x810
#define ACCEL_FORWARD_Y_UP_Z 0x802
#define ACCEL_FORWARD_Y_DOWN_Z 0x801
#define ACCEL_BACKWARD_Y_UP_X 0x420
#define ACCEL_BACKWARD_Y_DOWN_X 0x410
#define ACCEL_BACKWARD_Y_UP_Z 0x402
#define ACCEL_FORWARD_X_UP_Y 0x2008
#define ACCEL_FORWARD_X_DOWN_Y 0x2004
#define ACCEL_BACKWARD_X_UP_Y 0x1008
#define ACCEL_BACKWARD_X_DOWN_Y 0x1004
#define ACCEL_BACKWARD_X_UP_Z 0x1002
#define ACCEL_BACKWARD_X_DOWN_Z 0x1001

/* Debug Settings */
// #define DEBUG_ACCELEROMETER_OUTPUT_GYRO
// #define DEBUG_ACCELEROMETER_OUTPUT_ACCEL

#if defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO) or defined(DEBUG_ACCELEROMETER_OUTPUT_ACCEL)
int displayCount = 0;
#endif

SensorAccelerometer::SensorAccelerometer(AccelerometerController* controller) : _controller(controller) {
    _setTransformationMatrix(ACCEL_FORWARD_Z_UP_Y);
}

SensorAccelerometer::SensorAccelerometer(AccelerometerController* controller, uint16_t forward, uint16_t up) : _controller(controller) {
    _setTransformationMatrix((forward << 8) | up);
}

SensorAccelerometer::~SensorAccelerometer() { }

String SensorAccelerometer::getHumanName() {
    return "Accelerometer";
}

void SensorAccelerometer::begin() {
    _initialized = _controller->init();

    // get initial reading of pitch and gravitational acceleration on z and y axes.
    // assumes that the vehicle is not moving when this method is called
    if (_initialized) {
        if (_controller->tryGetReading()) {
            _accel = _transformationMatrix.multiply(_controller->getAccel());
            _gyro = _transformationMatrix.multiply(_controller->getGyro());
            _setPitch();
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
    if (_lastReadMillis + READ_INTERVAL < millis()) {
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
    if (_lastReadMillis + READ_INTERVAL < millis()) {
        _lastReadMillis = millis();
        success = _controller->tryGetReading();
    }
    #endif

    if (success) {
        _accel = _transformationMatrix.multiply(_controller->getAccel());
        _gyro = _transformationMatrix.multiply(_controller->getGyro());

        if (!_tryRecalibrateGyroscope()) {
            uint64_t currentTime = micros();
            uint64_t deltaT = currentTime - _lastPitchUpdateMicros;
            _lastPitchUpdateMicros = currentTime;
            _pitch = (_gyro.x * deltaT) + _pitch;
        }
        
        // update expected gravitational acceleration on y, z based on new pitch
        _setGravityY();
        _setGravityZ();
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
    return FLOAT_TO_STRING(_accel.z - _gravityZ, 2);
}

String SensorAccelerometer::getVerticalAcceleration(bool& valid) {
    valid = _initialized;
    return FLOAT_TO_STRING(_accel.y - _gravityY, 2);
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
    _gravityY = (GRAVITY * cos((float)_pitch / MEGA));
}

void SensorAccelerometer::_setGravityZ() {
    _gravityZ = GRAVITY * sin((float)_pitch / MEGA);
}

void SensorAccelerometer::_setPitch() {
    // <1,0> • <y,z> = |<y,z>| cos(t)  ->  t = arccos(y / |<y,z>|)
    _pitch = acos((_accel.y) / (sqrt(pow(_accel.y, 2) + pow(_accel.z, 2)))) *
        (_accel.z <= 0 ? -1 : 1) * MEGA;
    _lastPitchUpdateMicros = micros();
}

bool SensorAccelerometer::_tryRecalibrateGyroscope() {
    if (fabs((_accel.z * _accel.z + _accel.y * _accel.y) - (GRAVITY * GRAVITY)) <= GYRO_RECALIBRATION_MARGIN) {

        #if defined(DEBUG_ACCELEROMETER_OUTPUT_GYRO) or defined(DEBUG_ACCELEROMETER_OUTPUT_ACCEL)
        DEBUG_SERIAL_LN("RECALIBRATING GYROSCOPE!");
        #endif

        _setPitch();
        return true;
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
		case ACCEL_FORWARD_Z_UP_X:
			_transformationMatrix.setMatrix({{{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}}});
			break;
        default:
            break;
    }
}
