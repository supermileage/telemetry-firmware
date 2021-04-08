#include "Particle.h"
#include "Wire.h"
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "MicroNMEA.h"

class Sensor_GPS {
    public:
        Sensor_GPS(uint16_t updateInterval);

        void begin();
        void handle();

        double getLatitude();
        double getLongitude();
        double getAltitude();
        double getSpeed();
        uint8_t getNumSatellites();

    private:
        SFE_UBLOX_GNSS GPS;
        time_t _time;
        long unsigned int updateInterval;
        long unsigned int lastUpdate;
        
        double latitude;
        double longitude;
        double altitude;
        double speed;
        uint8_t SIV;
};