#ifndef _SENSOR_GPS_H_
#define _SENSOR_GPS_H_

#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "Sensor.h"

class SensorGps : public Sensor {
    public:

        /**
         * Constructor
         * 
         * @param *gps pointer to gpd object
         **/
        SensorGps(SFE_UBLOX_GNSS *gps);

        /**
         * Initialize the GPS sensor and i2c interface
         **/
        void begin() override;

        /**
         * Polls GPS for any new data. Run as frequently as possible 
         **/
        void handle() override;

        String getHumanName() override;

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
        String getLongitude(bool &valid = Sensor::dummy);

        /**
         * @return Latitude (degrees)
         **/
        String getLatitude(bool &valid = Sensor::dummy);

        /**
         * @return Heading of motion (degrees)
         **/
        int getHeading(bool &valid = Sensor::dummy);

        /**
         * @return Horizontal speed (m/s)
         **/
        String getHorizontalSpeed(bool &valid = Sensor::dummy);

        /**
         * @return Horizontal acceleration (m/s^2)
         **/
        String getHorizontalAcceleration(bool &valid = Sensor::dummy);

        /**
         * @return Horizontal position accuracy (m), max 10,000m
         **/
        String getHorizontalAccuracy(bool &valid = Sensor::dummy);

        /**
         * @return Vertical Position relative to Mean Sea Level (m)
         **/
        String getAltitude(bool &valid = Sensor::dummy);

        /**
         * @return Vertical speed (m/s)
         **/
        String getVerticalSpeed(bool &valid = Sensor::dummy);
        
        /**
         * @return Vertical acceleration b (m/s^2)
         **/
        String getVerticalAcceleration(bool &valid = Sensor::dummy);

        /**
         * @return Vertical position accuracy (m), max 10,000m
         **/
        String getVerticalAccuracy(bool &valid = Sensor::dummy);

		/**
		 * @brief Incline -- arctan(vertical distance / horizontal distance)
		 * 
		 */
        String getIncline(bool &valid = Sensor::dummy);

        /**
         * @return Number of Satellites currently seen by GPS
         **/
        int getSatellitesInView(bool &valid = Sensor::dummy);

        /**
         * @brief Update the callback function used by GPS to notify higher-level class of new GPS speed
         * 
         * @param speed() Pointer to function to call when there is a new speed
         **/
        void setSpeedCallback(void (*speed)(float));

        /**
         * @brief Toggle greenlist override
         **/
        void toggleOverride();

    private:
        SFE_UBLOX_GNSS* _gps;
        uint8_t _updateFrequency;

        bool _valid = false;

        uint64_t _lastUpdateMicros = 0;

        float _lastHorizontalSpeed = 0.0;
        float _horizontalAcceleration = 0.0;

        float _lastAltitude = 0.0;
        float _verticalSpeed = 0.0;
        float _lastVerticalSpeed = 0.0;
        float _verticalAcceleration = 0.0;
        float _horizontalDistance = 0.0;
        float _verticalDistance = 0.0;
        void (*_speedCallback)(float) = NULL;
        bool _override = true;

};

#endif
