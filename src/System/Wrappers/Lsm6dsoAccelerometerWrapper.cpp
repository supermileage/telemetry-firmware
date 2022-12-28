#include "Lsm6dsoAccelerometerWrapper.h"

Lsm6dsoAccelerometerWrapper::Lsm6dsoAccelerometerWrapper(SPIClass *spi, int csPin) :
    _spi(spi), _csPin(csPin) {
        _lsm6 = new Adafruit_LSM6DSOX();
    }

bool Lsm6dsoAccelerometerWrapper::init() { 
    _spi->begin();
    return _lsm6->begin_SPI(_csPin, _spi);
}

bool Lsm6dsoAccelerometerWrapper::tryGetReading() { 
    return _lsm6->getEvent(&_accel, &_gyro, &_temp);
}

Vec3 Lsm6dsoAccelerometerWrapper::getAccel() { 
    return Vec3 { _accel.acceleration.x, _accel.acceleration.y, _accel.acceleration.z };
}

Vec3 Lsm6dsoAccelerometerWrapper::getGyro() {
    return Vec3 { _gyro.gyro.x, _gyro.gyro.y, _gyro.gyro.z };
}

float Lsm6dsoAccelerometerWrapper::getTemp() { 
    return _temp.temperature;
}
