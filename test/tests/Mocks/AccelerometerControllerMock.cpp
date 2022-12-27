#include "AccelerometerControllerMock.h"

bool AccelerometerControllerMock::init() {
    return _init();
}

bool AccelerometerControllerMock::tryGetReading() {
    return _tryGetReading();
}

Vec3 AccelerometerControllerMock::getAccel() {
    return _getAccel();
}

Vec3 AccelerometerControllerMock::getGyro() {
    return _getGyro();
}

float AccelerometerControllerMock::getTemp() {
    return _getTemp();
}

void AccelerometerControllerMock::setInit(std::function<bool(void)> func) {
    _init = func;
}

void AccelerometerControllerMock::setTryGetReading(std::function<bool(void)> func) {
    _tryGetReading = func;
}

void AccelerometerControllerMock::setGetGyro(std::function<Vec3(void)> func) {
    _getGyro = func;
}

void AccelerometerControllerMock::setGetAccel(std::function<Vec3(void)> func) {
    _getAccel = func;
}

void AccelerometerControllerMock::setGetTemp(std::function<float(void)> func) {
    _getTemp = func;
}

void AccelerometerControllerMock::setReturnValues(Vec3 accel, Vec3 gyro, bool success) {
    _getAccel = [&accel]() { return accel; };
    _getGyro = [&gyro]() { return gyro; };
    _tryGetReading = [&success]() { return success; };
}
