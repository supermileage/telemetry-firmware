#ifndef _LSM6DSOX_LSM6DSOEROMETER_WRAPPER_H_
#define _LSM6DSOX_LSM6DSOEROMETER_WRAPPER_H_

#include "settings.h"
#include "Adafruit_LSM6DSOX.h"
#include "AccelerometerController.h"

#define LSM6DSO_POSITIVE_X 0x20
#define LSM6DSO_NEGATIVE_X 0x10
#define LSM6DSO_POSITIVE_Y 0x8
#define LSM6DSO_NEGATIVE_Y 0x4
#define LSM6DSO_POSITIVE_Z 0x2
#define LSM6DSO_NEGATIVE_Z 0x1

class Lsm6dsoAccelerometerWrapper : public AccelerometerController {
    public:
        Lsm6dsoAccelerometerWrapper(SPIClass *spi, int csPin, uint16_t forward, uint16_t up);
        bool init() override;
        bool tryGetReading() override;
        Vec3 getAccel() override;
        Vec3 getGyro() override;
        float getTemp() override;

    private:
        Adafruit_LSM6DS *_lsm6;
        SPIClass *_spi;
        int _csPin;
        Matrix3d _translationMatrix;
        sensors_event_t _accel;
        sensors_event_t _gyro;
        sensors_event_t _temp;

        void _setTranslationMatrix(uint16_t orientation);
};

#endif