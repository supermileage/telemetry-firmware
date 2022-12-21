#ifndef _SENSOR_ACCELEROMETER_H_
#define _SENSOR_ACCELEROMETER_H_

#include "Particle.h"
#include "Sensor.h"
#include "Adafruit_LSM6DSOX.h"

class SensorAccelerometer : public Sensor {
    public:
        SensorAccelerometer(SPIClass* spi, uint8_t csPin);
        ~SensorAccelerometer();
        String getHumanName() override;
        void begin() override;
        void handle() override;

        float getGyroX();
        float getGyroY();
        float getGyroZ();

        String getInitStatus();
    private:
        Adafruit_LSM6DSOX* _lsm6;
        SPIClass* _spi;
        uint8_t _csPin;
        sensors_event_t _accel;
        sensors_event_t _gyro;
        sensors_event_t _temp;
        bool _initialized = false;
};

#endif