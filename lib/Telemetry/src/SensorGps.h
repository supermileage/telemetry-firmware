#ifndef _SENSOR_GPS_H_
#define _SENSOR_GPS_H_

#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "Sensor.h"

class SensorGps : public Sensor {
    public:

        /**
         * Constructor
         * 
         * @param updateFrequency of GPS module in Hz [1,10] 
         **/
        SensorGps(uint8_t updateFrequency);

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
         * @return true if current time is valid
         **/
        bool getTimeValid();

        /**
         * @return UNIX Time (seconds)
         **/
        uint32_t getUnixTime();

        /**
         * @return Longitude (degrees)
         **/
        float getLongitude();

        /**
         * @return Latitude (degrees)
         **/
        float getLatitude();

        /**
         * @return Heading of motion (degrees)
         **/
        float getHeading();

        /**
         * @return Horizontal speed (m/s)
         **/
        float getXySpeed();

        /**
         * @return Horizontal acceleration between the last 2 times speed was polled (m/s^2)
         **/
        float getXyAcceleration();

        /**
         * @return Horizontal position accuracy (m)
         **/
        float getXyAccuracy();

        /**
         * @return Vertical Position relative to Mean Sea Level (m)
         **/
        float getAltitude();

        /**
         * @return Vertical speed between the last 2 times altitude was polled (m/s)
         **/
        float getZSpeed();
        
        /**
         * @return Vertical acceleration between the last 2 times altitude was polled (m/s^2)
         **/
        float getZAcceleration();

        /**
         * @return Vertical position accuracy (m)
         **/
        float getZAccuracy();

    private:
        SFE_UBLOX_GNSS* _gps;
        uint8_t _updateFrequency;

        uint64_t _lastUpdateMicros;

        float _lastXySpeed;
        float _xyAcceleration;

        float _lastZPosition;
        float _zSpeed;
        float _lastZSpeed;
        float _zAcceleration;

};

#endif
