#ifndef _SENSOR_GPS_H_
#define _SENSOR_GPS_H_

#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "Sensor.h"

class SensorGps : public Sensor {
    public:
        SensorGps(uint16_t updateInterval);

        void begin();
        void handle();
        
        String getSentence();
        float getSpeedKph();

    private:
        SFE_UBLOX_GNSS* _gps;
        uint16_t _updateInterval;

};

#endif
