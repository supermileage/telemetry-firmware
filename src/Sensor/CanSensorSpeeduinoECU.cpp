#include "settings.h"
#include "CanSensorSpeeduinoECU.h"

#define CALIBRATION_TEMPERATURE_OFFSET 25 //ask powertrain

const uint16_t SPEEDUINO_CAN_IDS[] {
	CAN_ECU_STATUS,
	CAN_ECU_SENSORS,
	CAN_ECU_CORRECTION,
	CAN_ECU_LOOPS,
	CAN_ECU_RPMDOT,
};

CanSensorSpeeduinoECU::CanSensorSpeeduinoECU(CanInterface& canInterface) : CanListener(canInterface) { }

CanSensorSpeeduinoECU::~CanSensorSpeeduinoECU() { }

void CanSensorSpeeduinoECU::begin() {
	for (uint16_t id : SPEEDUINO_CAN_IDS) {
		_canInterface.addMessageListen(id, new CanListener::CanListenerDelegate(this));
		_validation_speeduinoMap[id] = 0;
	}
}

void CanSensorSpeeduinoECU::handle() {
	if (_isAsleep)
		return;
}

uint64_t CanSensorSpeeduinoECU::getLastUpdateTime() {
	return _lastUpdateTime;
}

void CanSensorSpeeduinoECU::setIsAsleep(bool value) {
	_isAsleep = value;
}

bool CanSensorSpeeduinoECU::getIsAsleep() {
	return _isAsleep;
}

bool CanSensorSpeeduinoECU::_validate(uint16_t id) {
	return (millis() - _validation_speeduinoMap[id]) < STALE_INTERVAL;
}

String CanSensorSpeeduinoECU::getHumanName() {
	return "CanSensorSpeeduinoECU";
}

/* CAN_ECU_STATUS */
String CanSensorSpeeduinoECU::getcurrentStatus_secl(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_secl, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_squirt(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_squirt, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_engine(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_engine, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_dwell(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_dwell, 5);
}

String CanSensorSpeeduinoECU::getcurrentStatus_MAP(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_MAP, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_IAT(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_IAT, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_coolant(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_coolant, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_tpsADC(bool& valid) {
	valid = _validate(CAN_ECU_STATUS);
	return FLOAT_TO_STRING(_tpsADC, 1);
}

/* CAN_ECU_SENSORS */
String CanSensorSpeeduinoECU::getcurrentStatus_battery10(bool& valid) {
	valid = _validate(CAN_ECU_SENSORS);
	return FLOAT_TO_STRING(_battery10, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_O2(bool& valid) {
	valid = _validate(CAN_ECU_SENSORS);
	return FLOAT_TO_STRING(_O2, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_egoCorrection(bool& valid) {
	valid = _validate(CAN_ECU_SENSORS);
	return FLOAT_TO_STRING(_egoCorrection, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_iatCorrection(bool& valid) {
	valid = _validate(CAN_ECU_SENSORS);
	return FLOAT_TO_STRING(_iatCorrection, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_wueCorrection(bool& valid) {
	valid = _validate(CAN_ECU_SENSORS);
	return FLOAT_TO_STRING(_wueCorrection, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_RPM(bool& valid) {
	valid = _validate(CAN_ECU_SENSORS);
	return FLOAT_TO_STRING(_RPM, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_TAEamount(bool& valid) {
	valid = _validate(CAN_ECU_SENSORS);
	return FLOAT_TO_STRING(_TAEamount, 1);
}

/* CAN_ECU_CORRECTION */
String CanSensorSpeeduinoECU::getcurrentStatus_barometerCorrection(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_barometerCorrection, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_corrections(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_corrections, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_VE(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_VE, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_afrTarget(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_afrTarget, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_PW(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_PW, 5);
}

String CanSensorSpeeduinoECU::getcurrentStatus_tpsDOT(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_tpsDOT, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_advance(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_advance, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_TPS(bool& valid) {
	valid = _validate(CAN_ECU_CORRECTION);
	return FLOAT_TO_STRING(_TPS, 1);
}

/* CAN_ECU_LOOPS */
String CanSensorSpeeduinoECU::getcurrentStatus_loopsPerSecond(bool& valid) {
	valid = _validate(CAN_ECU_LOOPS);
	return FLOAT_TO_STRING(_loopsPerSecond_3103, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_freeRAM(bool& valid) {
	valid = _validate(CAN_ECU_LOOPS);
	return FLOAT_TO_STRING(_freeRAM_3103, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_batCorrection(bool& valid) {
	valid = _validate(CAN_ECU_LOOPS);
	return FLOAT_TO_STRING(_batCorrection_3103, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_spark(bool& valid) {
	valid = _validate(CAN_ECU_LOOPS);
	return FLOAT_TO_STRING(_spark_3103, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_O2_2(bool& valid) {
	valid = _validate(CAN_ECU_LOOPS);
	return FLOAT_TO_STRING(_O2_2_3103, 1);
}

/* CAN_ECU_RPMDOT */
String CanSensorSpeeduinoECU::getcurrentStatus_rpmDOT(bool& valid) {
	valid = _validate(CAN_ECU_RPMDOT);
	return FLOAT_TO_STRING(_rpmDOT, 1);
}

String CanSensorSpeeduinoECU::getcurrentStatus_flex(bool& valid) {
	valid = _validate(CAN_ECU_RPMDOT);
	return FLOAT_TO_STRING(_flex, 1);
}

void CanSensorSpeeduinoECU::update(CanMessage message) {
	_lastUpdateTime = millis();

	switch (message.id) {
		case CAN_ECU_STATUS:
			_secl = message.data[0];
			_squirt = message.data[1];
			_engine = message.data[2];
			_dwell = message.data[3] / 10.0f;
			_MAP = message.data[4] / 2.0f;
			_IAT = message.data[5] - CALIBRATION_TEMPERATURE_OFFSET;
			_coolant = message.data[6] - CALIBRATION_TEMPERATURE_OFFSET;
			_tpsADC = message.data[7];

			_validation_speeduinoMap[CAN_ECU_STATUS] = _lastUpdateTime;
			break;

		case CAN_ECU_SENSORS:
			// Note: battery10 is scaled as volts x 10.
			_battery10 = message.data[0] / 10.0f;
			_O2 = message.data[1];
			_egoCorrection = message.data[2];
			_iatCorrection = message.data[3];
			_wueCorrection = message.data[4];
			_RPM = (message.data[6] << 8) | message.data[5];
			_TAEamount = message.data[7];

			_validation_speeduinoMap[CAN_ECU_SENSORS] = _lastUpdateTime;
			break;

		case CAN_ECU_CORRECTION:
			_barometerCorrection = message.data[0];
			_corrections = message.data[1];
			_VE = message.data[2];
			_afrTarget = message.data[3];
			_PW = message.data[4] / 100.0f;
			_tpsDOT = message.data[5];
			_advance = message.data[6];
			_TPS = message.data[7];

			_validation_speeduinoMap[CAN_ECU_CORRECTION] = _lastUpdateTime;
			break;

		case CAN_ECU_LOOPS:
			_loopsPerSecond_3103 = (message.data[1] << 8) | message.data[0];
			_freeRAM_3103 = (message.data[3] << 8) | message.data[2];
			_batCorrection_3103 = message.data[4];
			_spark_3103 = message.data[5];
			_O2_2_3103 = message.data[6];

			_validation_speeduinoMap[CAN_ECU_LOOPS] = _lastUpdateTime;
			break;

		case CAN_ECU_RPMDOT:
			_rpmDOT = (message.data[2] << 8) | message.data[1];
			_flex = message.data[3];

			_validation_speeduinoMap[CAN_ECU_RPMDOT] = _lastUpdateTime;
			break;

		default:
			break;
	}
}
