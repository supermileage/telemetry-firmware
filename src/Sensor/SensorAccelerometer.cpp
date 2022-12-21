#include "SensorAccelerometer.h"

SensorAccelerometer::SensorAccelerometer(SPIClass* spi, uint8_t csPin) : _spi(spi), _csPin(csPin) {
    _lsm6 = new Adafruit_LSM6DSOX();
}

SensorAccelerometer::~SensorAccelerometer() {

}

String SensorAccelerometer::getHumanName() {
    return "Accelerometer";
}

void SensorAccelerometer::begin() {
    _spi->begin();
    _initialized = _lsm6->begin_SPI(_csPin, _spi);
}

void SensorAccelerometer::handle() {
    _lsm6->getEvent(&_accel, &_gyro, &_temp);
}

float SensorAccelerometer::getGyroX() {
    return _gyro.gyro.x;
}

float SensorAccelerometer::getGyroY() {
    return _gyro.gyro.y;
}

float SensorAccelerometer::getGyroZ() {
    return _gyro.gyro.z;
}

String SensorAccelerometer::getInitStatus() {
    if (_initialized) {
        return "Success";
    } else {
        return "Failure";
    }
}


