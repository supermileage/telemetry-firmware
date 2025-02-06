#ifndef _SENSOR_STEERING_H_
#define _SENSOR_STEERING_H_

#include "CanListener.h"

#define STEERING_READY_BIT_IGNITION 0x00
#define STEERING_READY_BIT_DMS      0x01
#define STEERING_READY_BIT_BRAKE    0x02

using namespace can;

class CanSensorSteering : public CanListener {
    public:    
        /**
         * @brief Constructor for CanSensorSteering
         * 
         * @param canInterface the can interface which will be reading data from Can buffer
         */
        CanSensorSteering(CanInterface &canInterface);

        /**
		 * @brief Overrides CanListenere's begin() to add multiple listen IDS
		 */
        void begin() override;

        /**
         * Check that values are still valid
         */
        void handle() override;
        
        /**
         * @brief Get the string name of this object
         */
        String getHumanName() override;

        /**
        * @return Throttle Position, %
        * */
        int getThrottle(bool& valid = Sensor::dummy);

        /**
        * @return Ignition Switch, Bool
        * */
        int getIgnition(bool& valid = Sensor::dummy);

        /**
        * @return Dead Man's Switch, Bool (on = hands on wheel)
        * */
        int getDms(bool& valid = Sensor::dummy);

        /**
        * @return Brake Switch, Bool
        * */
        int getBrake(bool& valid = Sensor::dummy);

    private:
        /**
         * @brief Receives CAN Message from Steering
         * 
         * @param message
         */
        void update(CanMessage message) override;

        uint32_t _lastUpdateThrottle = 0;
        uint32_t _lastUpdateReady = 0;

        uint8_t _throttle;
        int _ignition = Sensor::Status::Unknown;
        int _dms = Sensor::Status::Unknown;
        int _brake = Sensor::Status::Unknown;
};

#endif
