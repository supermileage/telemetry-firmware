#include "Particle.h"
#include "Sensor.h"

class Sensor_ECU: public Sensor {
    public:
        Sensor_ECU(USARTSerial *serial);

        void begin();
        void handle();

        time_t getTimestamp();
        float get(String field);

    private:
        USARTSerial * _serial;
        std::map<String, float> fields;
        
        time_t _time;
        float _interpretValue(uint8_t high, uint8_t low, float factor, float offset);
        void flush();
};