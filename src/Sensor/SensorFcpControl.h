// Header guard
#ifndef _SENSOR_FCP_CONTROL_H_
#define _SENSOR_FCP_CONTROL_H_

#include <stdint.h>
#include <vector>

#include "Sensor.h"
#include "TelemetrySerial.h"

/* Sensor which reads and interprets serial data sent from FCP Horizon Fuel Cell
 */
class SensorFcpControl : public Sensor {
    public:
        static const int32_t PacketSize;

        SensorFcpControl(TelemetrySerial *serial);
        SensorFcpControl();
        ~SensorFcpControl();
        String getHumanName() override;
        void begin() override;
        void handle() override;
        bool isConnected();
        String getErrorFlag(bool &valid = Sensor::dummy);
        String getAmbientTemperature(bool &valid = Sensor::dummy);
        String getFuelCellVoltage(bool &valid = Sensor::dummy);
        String getH2LeakVoltage(bool &valid = Sensor::dummy);
        String getFuelCellTemperature(bool &valid = Sensor::dummy);
        String getFuelCellCurrent(bool &valid = Sensor::dummy);
        String getBatteryVoltage(bool &valid = Sensor::dummy);

    private:
        TelemetrySerial *_serial;
        void _unpackData(uint8_t *buf);
        void _flushSerial();
        bool _checkErrHeader(uint8_t errorFlag);
        uint64_t _lastUpdate = 0;
        bool _valid = false;
        int _errorFlag = 0;
        float _ambientTemperature = 0.0;
        float _fuelCellVoltage = 0.0;
        float _h2LeakVoltage = 0.0;
        float _fuelCellTemperature = 0.0;
        float _fuelCellCurrent = 0.0;
        float _batteryVoltage = 0.0;
};

#endif
