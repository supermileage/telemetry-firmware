#include "SparkFun_u-blox_GNSS_Arduino_Library.h"

class SensorGps {
    public:
        SensorGps(uint16_t updateInterval);

        void begin();
        void handle();

        double getLatitude();
        double getLongitude();
        double getAltitude();
        double getSpeed();
        uint8_t getNumSatellites();

    private:
        SFE_UBLOX_GNSS* _gps;
        time_t _time;
        long unsigned int _updateInterval;
        long unsigned int _lastUpdate;
        
        double _latitude;
        double _longitude;
        double _altitude;
        double _speed;
        uint8_t _siv;
};