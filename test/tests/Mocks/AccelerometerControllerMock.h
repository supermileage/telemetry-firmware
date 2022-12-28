#ifndef _ACCELEROMETER_CONTROLLER_MOCK_H_
#define _ACCELEROMETER_CONTROLLER_MOCK_H_

#include <functional>
#include "AccelerometerController.h"

class AccelerometerControllerMock : public AccelerometerController {
    public:
        AccelerometerControllerMock();
        bool init() override;
        bool tryGetReading() override;
        Vec3 getAccel() override;
        Vec3 getGyro() override;
        float getTemp() override;

        void setInit(std::function<bool(void)> func);
        void setTryGetReading(std::function<bool(void)> func);
        void setGetGyro(std::function<Vec3(void)> func);
        void setGetAccel(std::function<Vec3(void)> func);
        void setGetTemp(std::function<float(void)> func);
        void setReturnValues(Vec3 accel, Vec3 gyro, bool success);

    private:
        std::function<bool(void)> _init;
        std::function<bool(void)> _tryGetReading;
        std::function<Vec3(void)> _getAccel;
        std::function<Vec3(void)> _getGyro;
        std::function<float(void)> _getTemp;
        
};

#endif