// Header guard
#ifndef _SENSOR_FCP_HORIZON_H_
#define _SENSOR_FCP_HORIZON_H_

#include <stdint.h>
#include <vector>

#include "Particle.h"
#include "Sensor.h"
#include "TelemetrySerial.h"

/* Sensor which reads and interprets serial data sent from FCP Horizon Fuel Cell */
class SensorFcpHorizon: public Sensor {
    public:
        static const int32_t PacketSize;

        SensorFcpHorizon(TelemetrySerial* serial);
        SensorFcpHorizon();
        ~SensorFcpHorizon();
        String getHumanName() override;
        void begin() override;
        void handle() override;
        String getAmbientTemperature(bool& valid = Sensor::dummy);
        String getFuelCellVoltage(bool& valid = Sensor::dummy);
        String getH2LeakVoltage(bool& valid = Sensor::dummy);
        String getFuelCellTemperature(bool& valid = Sensor::dummy);
        String getFuelCellCurrentHigh(bool& valid = Sensor::dummy);
        String getFuelCellCurrentLow(bool& valid = Sensor::dummy);
        String getBatteryVoltage(bool& valid = Sensor::dummy);

    private:
        TelemetrySerial* _serial;
        
        uint64_t _lastUpdate = 0;
        bool _valid = false;

        float _ambientTemperature = 0.0;
        float _fuelCellVoltage = 0.0;
        float _h2LeakVoltage = 0.0;
        float _fuelCellTemperature = 0.0;
        float _fuelCellCurrentHigh = 0.0;
        float _fuelCellCurrentLow = 0.0;
        float _batteryVoltage = 0.0;

        void _unpackData(uint8_t *buf);
        void _flushSerial();
};


#endif
