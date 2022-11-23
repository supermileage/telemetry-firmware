#ifndef _SENSOR_FCP_CELL_STACK_H_
#define _SENSOR_FCP_CELL_STACK_H_

#include <stdint.h>
#include <vector>

#include "Particle.h"
#include "Sensor.h"
#include "TelemetrySerial.h"

/* Sensor which reads and interprets serial data sent from FCP control board */
class SensorFcpControl: public Sensor {
    public:
        static const int32_t PacketSize;

        SensorFcpControl(TelemetrySerial* serial);
        ~SensorFcpControl();
        String getHumanName() override;
        void begin() override;
        void handle() override;
		int getNumFuelCells();
        String getNextCellVoltage(bool& valid = Sensor::dummy);
        float getCellVoltageByIndex(int index, bool& valid = Sensor::dummy);

    private:
        TelemetrySerial* _serial;
        std::vector<float> _cellVoltages;
        uint64_t _lastUpdate = 0;
        int8_t _lastCellVoltageIndex = -1;
		bool _valid = false;

        void _unpackCellVoltages(uint8_t *buf);
		void _flushSerial();
};

#endif