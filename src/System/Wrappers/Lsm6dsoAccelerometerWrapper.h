#ifndef _LSM6DSOX_ACCELEROMETER_WRAPPER_H_
#define _LSM6DSOX_ACCELEROMETER_WRAPPER_H_

#include "settings.h"
#include "Adafruit_LSM6DSOX.h"
#include "AccelerometerController.h"

class Lsm6dsoAccelerometerWrapper : public AccelerometerController {
    public:
        Lsm6dsoAccelerometerWrapper(SPIClass *spi, int csPin);
        bool init() override;
        bool tryGetReading() override;
        Vec3 getAccel() override;
        Vec3 getGyro() override;
        float getTemp() override;

    private:
        Adafruit_LSM6DS *_lsm6;
        SPIClass *_spi;
        int _csPin;
        sensors_event_t _accel;
        sensors_event_t _gyro;
        sensors_event_t _temp;

};

#endif