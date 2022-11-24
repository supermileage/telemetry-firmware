#include <iostream>

#include "test_config.h"

#include "CanInterface.h"
#include "CanControllerMock.h"
#include "CanSensorTinyBms.h"

/* Helper Functions */
void packBatteryDataTiny(float voltage, float current, float soc, uint8_t* buf);
void packCellDataTiny(float cellVoltageLow, float cellVoltageHigh, float cellVoltageAvg, uint8_t* buf);
void packTempDataTiny(int8_t packTempLow, int8_t packTempHigh, int8_t packTempAvg, int8_t bmsTemp, uint8_t* buf);
void packInt16Tiny(int16_t val, uint8_t* buf);

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

/* Tests */
TEST_CASE( "CanSensorTinyBms::getHumanName", "[CanSensorTinyBms][Sensor]" ) {
	CanInterface interface(nullptr);
	CanSensorTinyBms tiny(interface, 500);

	REQUIRE( tiny.getHumanName().equals("CanSensorTinyBms") );
}

TEST_CASE( "CanSensorTinyBms::update CAN_TINYBMS_REQUEST", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanControllerMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms tiny(interface, 500);

	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_TINYBMS_REQUEST;
	msg.dataLength = TINYBMS_REQ_DATA_LENGTH;

	Handler::instance().begin();

	SECTION( "YO" ) {
		
	}
}

TEST_CASE( "CanSensorTinyBms::update CAN_TINYBMS_TEMP", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanControllerMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms tiny(interface, 500);		//TEMP TESTS

	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = TINYBMS_PARAM_ID_TEMP;
	msg.dataLength = TINYBMS_REQ_DATA_LENGTH - 2;

	SECTION( "YO" ) {
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// assert
		bool bmsTempIsValid = false;
		REQUIRE ( tiny.getTempBms(bmsTempIsValid) == 24);
		REQUIRE ( bmsTempIsValid );
		REQUIRE ( tiny.getMaxBatteryTemp() == 0 );
		REQUIRE ( tiny.getAvgBatteryTemp() == 0 );
		REQUIRE ( tiny.getMinBatteryTemp() == 0 );

		testValidation(tiny, &CanSensorTinyBms::getTempBms);
	}
}

/* Helper Function Definitions */
void packBatteryDataTiny(float voltage, float current, float soc, uint8_t* buf) {
	packInt16Tiny((int16_t)(voltage * 10.0f), buf);
	packInt16Tiny((int16_t)(current * 10.0f), buf + 2);
	buf[4] = (uint8_t)(soc * 2.0f);
}

void packCellDataTiny(float cellVoltageLow, float cellVoltageHigh, float cellVoltageAvg, uint8_t* buf) {
	packInt16Tiny((int16_t)(cellVoltageLow * 1000), buf);
	packInt16Tiny((int16_t)(cellVoltageHigh * 1000), buf + 2);
	packInt16Tiny((int16_t)(cellVoltageAvg * 1000), buf + 4);
}

void packTempDataTiny(int8_t packTempLow, int8_t packTempHigh, int8_t packTempAvg, int8_t bmsTemp, uint8_t* buf) {
	buf[0] = packTempLow;
	buf[1] = packTempHigh;
	buf[2] = packTempAvg;
	buf[3] = bmsTemp;
}

void packInt16Tiny(int16_t val, uint8_t* buf) {
	*buf = val >> 8;
	*(buf + 1) = val & 0xFF;
}