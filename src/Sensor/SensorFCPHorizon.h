#ifndef _SENSOR_FCP_HORIZON_H_
#define _SENSOR_FCP_HORIZON_H_

#include <stdint.h>
#include <vector>

#include "Particle.h"
#include "Sensor.h"
#include "TelemetrySerial.h"

/* Sensor which reads and interprets serial data sent from FCP Horizon Fuel Cell */
class SensoryFCPHorizon: public Sensor {
    public:
        static const int32_t PacketSize;

        SensoryFCPHorizon(TelemetrySerial* serial);
        ~SensoryFCPHorizon();
        String getHumanName() override;
        void begin() override;
        void handle() override;
        String getAmbientTemperature(bool& valid = Sensor::dummy);
        String getFuelCellVoltage(bool& valid = Sensor::dummy);
        String getFuelCellTemperature(bool& valid = Sensor::dummy);
        String getFuelCellCurrentHigh(bool& valid = Sensor::dummy);
        String getFuelCellCurrentLow(bool& valid = Sensor::dummy);

    private:
        TelemetrySerial* _serial;
        
        uint64_t _lastUpdate = 0;
        bool _valid = false;

        void _unpackData(uint8_t *buf);
        void _flushSerial();
};


#endif
