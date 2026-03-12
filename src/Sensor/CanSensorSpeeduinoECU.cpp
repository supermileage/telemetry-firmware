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

CanSensorSpeeduinoECU::CanSensorSpeeduinoECU(CanInterface& canInterface) : CanSensorSpeeduinoECUtemp(canInterface) { }

CanSensorSpeeduinoECU::~CanSensorSpeeduinoECU() { }

void CanSensorSpeeduinoECU :: begin() { 
	for (uint16_t id : SPEEDUINO_CAN_IDS) {
		_canInterface.addMessageListen(id, new CanListener::CanListenerDelegate(this));
		_validation_speeduinoMap[id] = 0;
	}
}

String CanSensorSpeeduinoECU::getHumanName() {
	return "CanSensorSpeeduinoECU";
}

// ================= CAN 0x3100 =================
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

// ================= CAN 0x3101 =================
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

// ================= CAN 0x3102 =================
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

// ================= CAN 0x3103 =================
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

// ================= CAN 0x3104 =================
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
			_secl = message.data[0]; //secl is simply a counter that increments each second
			_squirt = message.data[1]; //Squirt Bitfield
			_engine = message.data[2]; //Engine Status Bitfield
			_dwell = message.data[3] / 10.0f; //Dwell in ms * 10
			_MAP = message.data[4] / 2.0f; //map value is divided by 2
			// I think offset was originally added, so you subract back
			_IAT = message.data[5] - CALIBRATION_TEMPERATURE_OFFSET; //mat
			_coolant = message.data[6] - CALIBRATION_TEMPERATURE_OFFSET; //Coolant ADC
			_tpsADC = message.data[7]; //TPS (Raw 0-255)

			_validation_speeduinoMap[CAN_ECU_STATUS] = _lastUpdateTime;
			break;

		case CAN_ECU_SENSORS:
		    // ================= CAN 3101 =================
			_battery10 = message.data[0]; //battery voltage
			_O2 = message.data[1]; //O2
			_egoCorrection = message.data[2]; //Exhaust gas correction (%)
			_iatCorrection = message.data[3]; //Air temperature Correction (%)
			_wueCorrection = message.data[4]; //Warmup enrichment (%)
			_RPM = (message.data[6] << 8) | message.data[5]; //rpm HB, rpm LB
			_TAEamount = message.data[7]; //acceleration enrichment (%)

			_validation_speeduinoMap[CAN_ECU_SENSORS] = _lastUpdateTime;
			break;
		    
		case CAN_ECU_CORRECTION:
			// ================= CAN 3102 =================
		    _barometerCorrection = message.data[0]; //Barometer correction (%)
			_corrections = message.data[1]; //Total GammaE (%)
			_VE = message.data[2]; //Current VE 1 (%)
			_afrTarget = message.data[3];
			_PW = message.data[4] / 10.0f; //Pulsewidth 1: ECU sends value = PW_us/100, PW_us = value * 100 * 1 ms / 1000 us = value / 10 (ms)
			_tpsDOT = message.data[5]; //TPS DOT
			_advance = message.data[6]; 
			_TPS = message.data[7]; // TPS (0% to 100%)

			_validation_speeduinoMap[CAN_ECU_CORRECTION] = _lastUpdateTime;
			break;

		case CAN_ECU_LOOPS:
			// ================= CAN 3103 =================
			_loopsPerSecond_3103 = (message.data[1] << 8) | message.data[0]; //LPS HB, LPS LB
			_freeRAM_3103 = (message.data[3] << 8) | message.data[2]; //freeRam HB, freeRam LB
			_batCorrection_3103 = message.data[4]; //Battery voltage correction (%)
			_spark_3103 = message.data[5]; //spark related bitfield
			_O2_2_3103 = message.data[6]; //Second O2
			//last byte unused

			_validation_speeduinoMap[CAN_ECU_LOOPS] = _lastUpdateTime;
			break;

		case CAN_ECU_RPMDOT:
		    // ================= CAN 3104 =================
			_rpmDOT = (message.data[2] << 8) | message.data[1]; //byte 0 unused, rpmDOT LB=byte1, HB=byte2
			_flex = message.data[3]; //Flex sensor value (or 0 if not used)

			_validation_speeduinoMap[CAN_ECU_RPMDOT] = _lastUpdateTime;
			break;

		default:
			// do nothing
			break;
	}
}