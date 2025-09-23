#ifndef _SENSOR_GPS_H_
#define _SENSOR_GPS_H_

#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "u-blox_config_keys.h"
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
         * @brief Turn the receiver's odometer feature on (CFG-ODO)
         * Ask the receiver to automatically send NAV-ODO messages
         * When a NAV-ODO arrives, our callback stores the latest distances
         * We write to all config layers (RAM/BBR/FLASH) so our GPS remembers our configuration settings if it supports this feature (maybe not on M8Q)
         */
        bool enableOdometer(bool enable = true, uint8_t layer = VAL_LAYER_ALL, uint16_t maxWait = defaultMaxWait);
        
        /**
         * @brief Ground distance since last reset (m)
         **/
        String getOdoDistance(bool &valid = Sensor::dummy);
        
        /**
         * @brief Total cumulative ground distance (m) since cold start
         **/
        String getOdoTotalDistance(bool &valid = Sensor::dummy);

        /**
         * @brief Ground distance 1-sigma accuracy (m)
         **/
        String getOdoDistanceStd(bool &valid = Sensor::dummy);

        /**
         * @brief Reset ground distance and accuracy (NAV-RESETODO). Returns true on ACK.
         */
        bool resetOdometer();

        /**
         * @brief Toggle greenlist override
         **/
        void toggleOverride();

        /**
         * @brief Query if greenlist override is enabled
         */
        bool isOverrideEnabled() const { return _override; }

            String getInitStatus();

    private:
        SFE_UBLOX_GNSS* _gps;
        uint8_t _updateFrequency;

        bool _valid = false;
		bool _initialized = false;

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
        // NOTE: PLEASE RESTORE TO FALSE IN PRODUCTION !!! Unless you want to get doxxed I guess.
        bool _override = true;

        // UBX-NAV-ODO cache
        static void navOdoCallback(UBX_NAV_ODO_data_t data);
        static SensorGps* _instance; // Simple singleton pointer for callback wiring
        uint32_t _odoDistance = 0;      // meters since last reset
        uint32_t _odoTotalDistance = 0; // meters since cold start
        uint32_t _odoDistanceStd = 0;   // meters, 1-sigma
        bool _odoAvailable = false;
        bool _odoEnabled = false;       // last known result of enabling ODO
        unsigned long _lastOdoPoll = 0; // ms since last NAV-ODO poll while waiting for first data
};

#endif
