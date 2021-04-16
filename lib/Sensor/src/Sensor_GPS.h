#include "Particle.h"
#include "Sensor.h"
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
// #include "MicroNMEA.h"

class Sensor_GPS: public Sensor {
    public:
        Sensor_GPS();

        void begin();
        void handle();

        time_t getTimestamp();

    private:
        SFE_UBLOX_GNSS GPS;
        char nmeaBuffer[100];
        // MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));
        time_t _time;

};