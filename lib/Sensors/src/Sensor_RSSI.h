#include "Particle.h"
#include "Arduino.h"

class Sensor_RSSI {
    public:
        Sensor_RSSI();
        int get();

    private:
        CellularSignal sig;

};