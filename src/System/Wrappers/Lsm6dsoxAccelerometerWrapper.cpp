#include "Lsm6dsoxAccelerometerWrapper.h"

Lsm6dsoxAccelerometerWrapper::Lsm6dsoxAccelerometerWrapper(Adafruit_LSM6DS *lsm6, SPIClass *spi, int csPin) :
    _lsm6(lsm6), _spi(spi), _csPin(csPin) { }

bool Lsm6dsoxAccelerometerWrapper::init() { 
    _spi->begin();
    return _lsm6->begin_SPI(_csPin, _spi);
}

bool Lsm6dsoxAccelerometerWrapper::tryGetReading() { 
    return _lsm6->getEvent(&_accel, &_gyro, &_temp);
}

Vec3 Lsm6dsoxAccelerometerWrapper::getAccel() { 
    return Vec3 { _accel.acceleration.x, _accel.acceleration.y, _accel.acceleration.z };
}

Vec3 Lsm6dsoxAccelerometerWrapper::getGyro() {
    return Vec3 { _gyro.gyro.x, _gyro.gyro.y, _gyro.gyro.z };
}

float Lsm6dsoxAccelerometerWrapper::getTemp() { 
    return _temp.temperature;
}
