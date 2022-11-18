#ifndef _SENSOR_FC_H_
#define _SENSOR_FC_H_

#include "Sensor.h"
#include "TelemetrySerial.h"

#define FC_BUFFER_SIZE 512

// Fuel Cell Controller sensor - Temporary, can't be used in competition.
// The nice thing about this sensor is that unless we hook up the specific
// hardware for it, this sensor class essentially does nothing and can be left in. 
class SensorFc : public Sensor {
    public:
        /**
         * Constructor 
         * @param *serial bus receiving Fuel Cell Controller data
         **/
        SensorFc(USARTSerial *serial);

        /**
         * Init serial port
         * */
        void begin() override;
        
        /**
         * Check serial buffer for new updates
         * */
        void handle() override;

        String getHumanName() override;


    private:
        USARTSerial * _serial;
        char _buffer[FC_BUFFER_SIZE];
        uint32_t _lastUpdate = 0;

        /**
         * Print to serial and publish message to Particle Cloud with buffer contents
         * */
        void _publish();

        /**
         * Empty the buffer
         * */
        void _emptyBuffer();
};

#endif
