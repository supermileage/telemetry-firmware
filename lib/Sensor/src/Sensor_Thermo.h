#include "Particle.h"
#include "SparkFunMAX31855k.h"

class Sensor_Thermo {
    public:
        Sensor_Thermo(uint8_t csPin);

        void begin();
        void handle();

    private:
        
};