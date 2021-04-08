#include "map"

class Sensor {
    public:
        virtual void begin() = 0;
        virtual void handle() = 0;

        virtual time_t getTimestamp() = 0;
};