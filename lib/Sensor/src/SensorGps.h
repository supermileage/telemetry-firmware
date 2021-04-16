#include "SparkFun_u-blox_GNSS_Arduino_Library.h"

class SensorGps {
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