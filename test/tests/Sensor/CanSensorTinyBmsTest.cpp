#include <iostream>

#include "test_config.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorTinyBms.h"

/* Helper Functions */
void packBatteryData(float voltage, float current, float soc, uint8_t* buf);
void packCellData(float cellVoltageLow, float cellVoltageHigh, float cellVoltageAvg, uint8_t* buf);
void packTempData(int8_t packTempLow, int8_t packTempHigh, int8_t packTempAvg, int8_t bmsTemp, uint8_t* buf);
void packInt16(int16_t val, uint8_t* buf);

template <typename T>
void testValidation(CanSensorTinyBms& tiny, T (CanSensorTinyBms::*getter)(bool&)) {
	bool statusIsValid = false;
	setMillis(DEFAULT_STALE_TIME_MILLIS - 1);
	(tiny.*getter)(statusIsValid);
	REQUIRE ( statusIsValid );

	setMillis(DEFAULT_STALE_TIME_MILLIS);
	(tiny.*getter)(statusIsValid);
	REQUIRE_FALSE ( statusIsValid );
}

// Ordered Faults unlikely but here anyway
const int orderedFaults[] = {};

/* Tests */
TEST_CASE( "CanSensorTinyBms::getHumanName", "[CanSensorTinyBms][Sensor]" ) {
	CanInterface interface(nullptr);
	CanSensorTinyBms tiny(interface, 500);

	REQUIRE( tiny.getHumanName().equals("CanSensorTinyBms") );
}

TEST_CASE( "CanSensorTinyBms::update CAN_TINYBMS_REQUEST", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms tiny(interface, 500);

	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_TINYBMS_REQUEST;
	msg.dataLength = TINYBMS_REQ_DATA_LENGTH;

	Handler::instance().begin();

	SECTION( "YO" ) {

		//testValidation(tiny, &CanSensorTinyBms::get);
	}
}

TEST_CASE( "CanSensorTinyBms::update CAN_TINYBMS_TEMP", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms tiny(interface, 500);		//TEMP TESTS

	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = TINYBMS_PARAM_ID_TEMP;
	msg.dataLength = TINYBMS_REQ_DATA_LENGTH - 2;

	SECTION( "YO" ) {

		// set up pack message with battery voltage value
		packTempData(0, 0, 24, 0, test);
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// assert
		bool minTempIsValid = false;
		REQUIRE ( tiny.getTempBms(bmsTempIsValid) == 24);
		REQUIRE ( bmsTempIsValid );
		REQUIRE ( orion.getMaxBatteryTemp() == 0 );
		REQUIRE ( orion.getAvgBatteryTemp() == 0 );
		REQUIRE ( orion.getMinBatteryTemp() == 0 );

		testValidation(tiny, &CanSensorTinyBms::getTempBms);
	}
}

/* Helper Function Definitions */
void packBatteryData(float voltage, float current, float soc, uint8_t* buf) {
	packInt16((int16_t)(voltage * 10.0f), buf);
	packInt16((int16_t)(current * 10.0f), buf + 2);
	buf[4] = (uint8_t)(soc * 2.0f);
}

void packCellData(float cellVoltageLow, float cellVoltageHigh, float cellVoltageAvg, uint8_t* buf) {
	packInt16((int16_t)(cellVoltageLow * 1000), buf);
	packInt16((int16_t)(cellVoltageHigh * 1000), buf + 2);
	packInt16((int16_t)(cellVoltageAvg * 1000), buf + 4);
}

void packTempData(int8_t packTempLow, int8_t packTempHigh, int8_t packTempAvg, int8_t bmsTemp, uint8_t* buf) {
	buf[0] = packTempLow;
	buf[1] = packTempHigh;
	buf[2] = packTempAvg;
	buf[3] = bmsTemp;
}

void packInt16(int16_t val, uint8_t* buf) {
	*buf = val >> 8;
	*(buf + 1) = val & 0xFF;
}