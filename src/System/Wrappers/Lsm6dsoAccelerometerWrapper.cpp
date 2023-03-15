#include "Lsm6dsoAccelerometerWrapper.h"

#define LSM6DSO_FORWARD_Z_UP_Y 0x208
#define LSM6DSO_FORWARD_Z_DOWN_Y 0x204
#define LSM6DSO_FORWARD_Z_UP_X 0x220
#define LSM6DSO_FORWARD_Z_DOWN_X 0x210
#define LSM6DSO_BACKWARD_Z_UP_Y 0x108
#define LSM6DSO_BACKWARD_Z_DOWN_Y 0x104
#define LSM6DSO_BACKWARD_Z_UP_X 0x120
#define LSM6DSO_BACKWARD_Z_DOWN_X 0x110

#define LSM6DSO_FORWARD_Y_UP_X 0x820
#define LSM6DSO_FORWARD_Y_DOWN_X 0x810
#define LSM6DSO_FORWARD_Y_UP_Z 0x802
#define LSM6DSO_FORWARD_Y_DOWN_Z 0x801
#define LSM6DSO_BACKWARD_Y_UP_X 0x420
#define LSM6DSO_BACKWARD_Y_DOWN_X 0x410
#define LSM6DSO_BACKWARD_Y_UP_Z 0x402
#define LSM6DSO_BACKWARD_Y_DOWN_Z 0x401

#define LSM6DSO_FORWARD_X_UP_Y 0x2008
#define LSM6DSO_FORWARD_X_DOWN_Y 0x2004
#define LSM6DSO_FORWARD_X_UP_Z 0x2002
#define LSM6DSO_FORWARD_X_DOWN_Z 0x2001
#define LSM6DSO_BACKWARD_X_UP_Y 0x1008
#define LSM6DSO_BACKWARD_X_DOWN_Y 0x1004
#define LSM6DSO_BACKWARD_X_UP_Z 0x1002
#define LSM6DSO_BACKWARD_X_DOWN_Z 0x1001

sensors_event_t Lsm6dsoAccelerometerWrapper::s_accel;
sensors_event_t Lsm6dsoAccelerometerWrapper::s_gyro;
sensors_event_t Lsm6dsoAccelerometerWrapper::s_temp;

Lsm6dsoAccelerometerWrapper::Lsm6dsoAccelerometerWrapper(SPIClass *spi, int csPin, uint16_t forward, uint16_t up) :
    _spi(spi), _csPin(csPin) {
        _lsm6 = new Adafruit_LSM6DSOX();
        _setGlobalTransform(((forward << 8) & up));
}

bool Lsm6dsoAccelerometerWrapper::init() { 
    _spi->begin();
    return _lsm6->begin_SPI(_csPin, _spi);
}

bool Lsm6dsoAccelerometerWrapper::tryGetReading() { 
    return _lsm6->getEvent(&s_accel, &s_gyro, &s_temp);
}

Vec3 Lsm6dsoAccelerometerWrapper::getAccel() { 
    return Vec3 { (_getGlobalAccelX)(), (_getGlobalAccelX)(), (_getGlobalAccelZ)() };
}

Vec3 Lsm6dsoAccelerometerWrapper::getGyro() {
    return Vec3 { (_getGlobalGyroX)(), (_getGlobalGyroY)(), (_getGlobalGyroZ)() };
}

float Lsm6dsoAccelerometerWrapper::getTemp() { 
    return s_temp.temperature;
}

// NOTE: there are so many configurations to consider that I've only added the ones we
//       currently need.  This can be fleshed out later
void Lsm6dsoAccelerometerWrapper::_setGlobalTransform(uint16_t orientation) {
    switch (orientation) {
        case LSM6DSO_FORWARD_Z_UP_Y:
            _getGlobalAccelX = []() { return s_accel.acceleration.x; };
            _getGlobalAccelY = []() { return s_accel.acceleration.y; };
            _getGlobalAccelZ = []() { return s_accel.acceleration.z; };
            _getGlobalGyroX = []() { return s_gyro.gyro.x; };
            _getGlobalGyroY = []() { return s_gyro.gyro.y; };
            _getGlobalGyroZ = []() { return s_gyro.gyro.z; };
            break;
        case LSM6DSO_FORWARD_Z_DOWN_Y:
            _getGlobalAccelX = []() { return -s_accel.acceleration.x; };
            _getGlobalAccelY = []() { return -s_accel.acceleration.y; };
            _getGlobalAccelZ = []() { return s_accel.acceleration.z; };
            _getGlobalGyroX = []() { return -s_gyro.gyro.x; };
            _getGlobalGyroY = []() { return s_gyro.gyro.y; };
            _getGlobalGyroZ = []() { return s_gyro.gyro.z; };
            break;
        case LSM6DSO_FORWARD_X_UP_Z:
            _getGlobalAccelX = []() { return s_accel.acceleration.y; };
            _getGlobalAccelY = []() { return s_accel.acceleration.z; };
            _getGlobalAccelZ = []() { return s_accel.acceleration.x; };
            _getGlobalGyroX = []() { return s_gyro.gyro.y; };
            _getGlobalGyroY = []() { return s_gyro.gyro.z; };
            _getGlobalGyroZ = []() { return s_gyro.gyro.x; };
            break;
        case LSM6DSO_FORWARD_X_DOWN_Z:
            _getGlobalAccelX = []() { return s_accel.acceleration.y; };
            _getGlobalAccelY = []() { return -s_accel.acceleration.z; };
            _getGlobalAccelZ = []() { return s_accel.acceleration.x; };
            _getGlobalGyroX = []() { return s_gyro.gyro.y; };
            _getGlobalGyroY = []() { return -s_gyro.gyro.z; };
            _getGlobalGyroZ = []() { return s_gyro.gyro.x; };
            break;
        default:
            break;
    }
}
