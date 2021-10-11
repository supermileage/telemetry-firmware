#ifndef _SENSOR_GPS_H_
#define _SENSOR_GPS_H_

#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "Sensor.h"

class SensorGps : public Sensor {
    public:

        /**
         * Constructor 
         * 
         * @param updateInterval for GPS position/speed in ms
         **/
        SensorGps(uint16_t updateInterval);

        /**
         * Initialize the GPS sensor and i2c interface
         **/
        void begin();

        /**
         * Polls GPS for any new data. Run as frequently as possible 
         **/
        void handle();

        String getHumanName();
        
        /**
         * @return GPRMC/GNRMC sentence containing position and other data
         **/
        String getSentence();

        /**
         * @return Speed in Kilometers per Hour
         **/
        float getSpeedKph();

        /**
         * @return UNIX Time
         **/
        uint32_t getTime();

        /**
         * @return If time has been received from satellite
         **/
        bool getTimeValid();

    private:
        SFE_UBLOX_GNSS* _gps;
        uint16_t _updateInterval;

};

#endif
