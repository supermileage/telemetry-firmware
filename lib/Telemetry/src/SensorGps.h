#ifndef _SENSOR_GPS_H_
#define _SENSOR_GPS_H_

#include "constants.h"
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "Sensor.h"

class SensorGps : public Sensor {
    public:

        /**
         * Constructor
         * 
         * @param updateFrequency of GPS module in Hz [1,10]. Each update takes 5-7ms. 
         **/
        SensorGps(uint8_t updateFrequency);

        /**
         * Destructor
         **/
        ~SensorGps();

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
        float getHorizontalSpeed();

        /**
         * @return Horizontal acceleration (m/s^2)
         **/
        float getHorizontalAcceleration();

        /**
         * @return Horizontal position accuracy (m)
         **/
        float getHorizontalAccuracy();

        /**
         * @return Vertical Position relative to Mean Sea Level (m)
         **/
        float getAltitude();

        /**
         * @return Vertical speed (m/s)
         **/
        float getVerticalSpeed();
        
        /**
         * @return Vertical acceleration b (m/s^2)
         **/
        float getVerticalAcceleration();

        /**
         * @return Vertical position accuracy (m)
         **/
        float getVerticalAccuracy();

    private:
        SFE_UBLOX_GNSS* _gps;
        uint8_t _updateFrequency;

        uint64_t _lastUpdateMicros = 0;

        float _lastHorizontalSpeed = 0.0;
        float _horizontalAcceleration = 0.0;

        float _lastAltitude = 0.0;
        float _verticalSpeed = 0.0;
        float _lastVerticalSpeed = 0.0;
        float _verticalAcceleration = 0.0;

};

#endif
