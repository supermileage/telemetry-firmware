#include <iostream>

#include "test_config.h"

#include "CanInterface.h"
#include "CanControllerMock.h"
#include "CanSensorTinyBms.h"

#define TEST_REQUEST_INTERVAL 500

/* Helper Functions */
void packBatteryDataTiny(float voltage, float current, float soc, uint8_t* buf);
void packCanMessageTiny(uint8_t status, uint8_t param, uint8_t len);
void packTempMessageTiny(CanMessage& msg, int16_t temp, uint8_t tempId);
void packCellDataTiny(float cellVoltageLow, float cellVoltageHigh, float cellVoltageAvg, uint8_t* buf);
void packInt16Tiny(int16_t val, uint8_t* buf);
void packInt32Tiny(int32_t val, uint8_t* buf);

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
	CanSensorTinyBms tiny(interface, TEST_REQUEST_INTERVAL);

	REQUIRE( tiny.getHumanName().equals("CanSensorTinyBms") );
}

TEST_CASE( "CanSensorTinyBms::handle CAN_TINYBMS_REQUEST", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanControllerMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms tiny(interface, TEST_REQUEST_INTERVAL);

	setMillis(DEFAULT_START_TIME_MILLIS);
	Handler::instance().begin();

	SECTION("should pass -- all TinyBms paramIds sent in correct order") {
		// iterate through all different tiny bms params
		for (int i = 0; i < TINYBMS_NUM_PARAMS; i++) {
			bool sendMsgBufferCalled = false;
			uint8_t requestParam = ~CanSensorTinyBms::ParamIds[i];

			// set time to trigger TinyBms to send new request
			setMillis(DEFAULT_START_TIME_MILLIS + (i + 1) * TEST_REQUEST_INTERVAL);

			// pass lambda to check if canBusMock::sendMessage() is being called
			canBusMock.setSendMsgBuffer([&sendMsgBufferCalled, &requestParam](unsigned long id, byte ext, byte len, const byte *buf) {
				if (id == CAN_TINYBMS_REQUEST) {
					sendMsgBufferCalled = true;
					requestParam = buf[0];
				}
			});

			tiny.handle();

			REQUIRE( sendMsgBufferCalled );
			REQUIRE( requestParam == CanSensorTinyBms::ParamIds[i]);
		}
	}

	SECTION("should fail -- won't update until update interval has passed") {
		bool sendMsgBufferCalled = false;

		// set tinybms last update to current time
		tiny.handle();

		// set time to trigger TinyBms to send new request
		setMillis(DEFAULT_START_TIME_MILLIS + TEST_REQUEST_INTERVAL - 1);

		// pass lambda to check if canBusMock::sendMessage() is being called
		canBusMock.setSendMsgBuffer([&sendMsgBufferCalled](unsigned long id, byte ext, byte len, const byte *buf) {
			if (id == CAN_TINYBMS_REQUEST) {
				sendMsgBufferCalled = true;
			}
		});

		tiny.handle();

		REQUIRE_FALSE( sendMsgBufferCalled );
	}

	SECTION("should pass -- SOC update sent when handle called") {
		bool sendMsgBufferCalled = false;

		// pass lambda to check if canBusMock::sendMessage() is being called
		canBusMock.setSendMsgBuffer([&sendMsgBufferCalled](unsigned long id, byte ext, byte len, const byte *buf) {
			if (id == CAN_TELEMETRY_BMS_DATA) {
				sendMsgBufferCalled = true;
			}
		});

		tiny.handle();

		REQUIRE( sendMsgBufferCalled );
	}
}

TEST_CASE( "CanSensorTinyBms::update", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanControllerMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms tiny(interface, TEST_REQUEST_INTERVAL);

	setMillis(DEFAULT_START_TIME_MILLIS);

	Handler::instance().begin();

	SECTION( "should pass -- update can tinybms internal temp" ) {
		int testTemp = -15;

		CanMessage msg = CAN_MESSAGE_NULL;
		packTempMessageTiny(msg, testTemp, TINYBMS_TEMP_ID_INTERNAL);
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// assert
		bool bmsTempIsValid = false;
		REQUIRE ( tiny.getTempBms(bmsTempIsValid) == testTemp);
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

void packInt16Tiny(int16_t val, uint8_t* buf) {
	*(buf + TINYBMS_RSP_DATA_BYTE) = val & 0xFF;
	*(buf + TINYBMS_RSP_DATA_BYTE + 1) = val >> 8;
}

void packInt32Tiny(int32_t val, uint8_t* buf) {
	*(buf + TINYBMS_RSP_DATA_BYTE) = val & 0xFF;
	*(buf + TINYBMS_RSP_DATA_BYTE + 1) = (val >> 8) & 0xFF;
	*(buf + TINYBMS_RSP_DATA_BYTE + 2) = (val >> 16) & 0xFF;
	*(buf + TINYBMS_RSP_DATA_BYTE + 3) = (val >> 24) & 0xFF;
}

void packFloatTiny(float val, uint8_t* buf) {
	*(float*)(buf + TINYBMS_RSP_DATA_BYTE) = val;
}

void packCanMessageTiny(CanMessage& msg, uint8_t status, uint8_t param, uint8_t len) {
	msg.id = CAN_TINYBMS_RESPONSE;
	msg.data[TINYBMS_RSP_STATUS_BYTE] = status;
	msg.data[TINYBMS_RSP_PARAM_ID_BYTE] = param;
	msg.dataLength = len;
}

void packTempMessageTiny(CanMessage& msg, int16_t temp, uint8_t tempId) {
	packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_TEMP, TINYBMS_REQ_DATA_LENGTH - 2);
	msg.data[TINYBMS_TEMP_ID_BYTE] = tempId;
	packInt16Tiny(temp * TINYBMS_TEMP_SCALING_FACTOR, msg.data + 1);
}