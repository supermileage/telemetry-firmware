#ifndef _CAN_SENSOR_SPEEDUINO_ECU_H_
#define _CAN_SENSOR_SPEEDUINO_ECU_H_

#include <unordered_map>

#include "CanListener.h"
#include "CanInterface.h"
#include "can_common.h"

/**
 * Speeduino ECU over CAN (IDs 0x100–0x104). Single concrete sensor; inherits CanListener directly.
 *
 * Byte layout reference (Speeduino realtime CAN):
 *   0x100 — secl, squirt, engine, dwell, MAP/2, IAT+offset, coolant+offset, tpsADC
 *   0x101 — battery, O2, ego/iat/wue correction, RPM LB/HB, TAE
 *   0x102 — barometer, gammaE, VE, AFR target, PW, tpsDOT, advance, TPS
 *   0x103 — loops/s, free RAM, bat correction, spark, O2_2
 *   0x104 — rpmDOT, flex (see update() for exact byte indices)
 */
class CanSensorSpeeduinoECU : public CanListener {
	public:
		CanSensorSpeeduinoECU(CanInterface& canInterface);
		~CanSensorSpeeduinoECU();

		void begin() override;
		void handle() override;

		String getHumanName() override;

		uint64_t getLastUpdateTime();
		void setIsAsleep(bool value);
		bool getIsAsleep();

		/* CAN_ECU_STATUS */
		String getcurrentStatus_secl(bool& valid = Sensor::dummy);
		String getcurrentStatus_squirt(bool& valid = Sensor::dummy);
		String getcurrentStatus_engine(bool& valid = Sensor::dummy);
		String getcurrentStatus_dwell(bool& valid = Sensor::dummy);
		String getcurrentStatus_MAP(bool& valid = Sensor::dummy);
		String getcurrentStatus_IAT(bool& valid = Sensor::dummy);
		String getcurrentStatus_coolant(bool& valid = Sensor::dummy);
		String getcurrentStatus_tpsADC(bool& valid = Sensor::dummy);

		/* CAN_ECU_SENSORS */
		String getcurrentStatus_battery10(bool& valid = Sensor::dummy);
		String getcurrentStatus_O2(bool& valid = Sensor::dummy);
		String getcurrentStatus_egoCorrection(bool& valid = Sensor::dummy);
		String getcurrentStatus_iatCorrection(bool& valid = Sensor::dummy);
		String getcurrentStatus_wueCorrection(bool& valid = Sensor::dummy);
		String getcurrentStatus_RPM(bool& valid = Sensor::dummy);
		String getcurrentStatus_TAEamount(bool& valid = Sensor::dummy);

		/* CAN_ECU_CORRECTION */
		String getcurrentStatus_barometerCorrection(bool& valid = Sensor::dummy);
		String getcurrentStatus_corrections(bool& valid = Sensor::dummy);
		String getcurrentStatus_VE(bool& valid = Sensor::dummy);
		String getcurrentStatus_afrTarget(bool& valid = Sensor::dummy);
		String getcurrentStatus_PW(bool& valid = Sensor::dummy);
		String getcurrentStatus_tpsDOT(bool& valid = Sensor::dummy);
		String getcurrentStatus_advance(bool& valid = Sensor::dummy);
		String getcurrentStatus_TPS(bool& valid = Sensor::dummy);

		/* CAN_ECU_LOOPS */
		String getcurrentStatus_loopsPerSecond(bool& valid = Sensor::dummy);
		String getcurrentStatus_freeRAM(bool& valid = Sensor::dummy);
		String getcurrentStatus_batCorrection(bool& valid = Sensor::dummy);
		String getcurrentStatus_spark(bool& valid = Sensor::dummy);
		String getcurrentStatus_O2_2(bool& valid = Sensor::dummy);

		/* CAN_ECU_RPMDOT */
		String getcurrentStatus_rpmDOT(bool& valid = Sensor::dummy);
		String getcurrentStatus_flex(bool& valid = Sensor::dummy);

	private:
		float _secl = 0;
		float _squirt = 0;
		float _engine = 0;
		float _dwell = 0;
		float _MAP = 0;
		float _IAT = 0;
		float _coolant = 0;
		float _tpsADC = 0;

		float _battery10 = 0;
		float _O2 = 0;
		float _egoCorrection = 0;
		float _iatCorrection = 0;
		float _wueCorrection = 0;
		float _RPM = 0;
		float _TAEamount = 0;

		float _barometerCorrection = 0;
		float _corrections = 0;
		float _VE = 0;
		float _afrTarget = 0;
		float _PW = 0;
		float _tpsDOT = 0;
		float _advance = 0;
		float _TPS = 0;

		float _loopsPerSecond_3103 = 0;
		float _freeRAM_3103 = 0;
		float _batCorrection_3103 = 0;
		float _spark_3103 = 0;
		float _O2_2_3103 = 0;

		float _rpmDOT = 0;
		float _flex = 0;

		void update(CanMessage message) override;

		std::unordered_map<uint16_t, uint64_t> _validation_speeduinoMap;
		uint64_t _lastUpdateTime = 0;
		bool _isAsleep = false;

		bool _validate(uint16_t id);
};

#endif
