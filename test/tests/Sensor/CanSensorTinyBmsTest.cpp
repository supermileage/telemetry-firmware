#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <array>
#include <math.h>

#include "test_config.h"

#include "CanInterface.h"
#include "CanControllerMock.h"
#include "CanSensorTinyBms.h"

#define TEST_REQUEST_INTERVAL 500

using namespace std;

/* Helper Functions */
void packBatteryDataTiny(float voltage, float current, float soc, uint8_t* buf);
void packCanMessageTiny(CanMessage& msg, uint8_t status, uint8_t id, uint8_t len);
void packTempMessageTiny(CanMessage& msg, int16_t temp, uint8_t tempId);
void packFloatMessageTiny(CanMessage& msg, float val, uint8_t id);
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

/* Ordered Statuses */
const array<int, 6> Statuses {
	TINYBMS_STATUS_CHARGING, TINYBMS_STATUS_CHARGED, TINYBMS_STATUS_DISCHARGING,
	TINYBMS_STATUS_REGENERATION, TINYBMS_STATUS_IDLE, TINYBMS_STATUS_FAULT_ERROR
};

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
	CanMessage msg = CAN_MESSAGE_NULL;

	setMillis(DEFAULT_START_TIME_MILLIS);

	Handler::instance().begin();

	SECTION("update sets _lastUpdateTime") {
		uint64_t arbitraryUpdateTime = 150;

		setMillis(arbitraryUpdateTime);
		packCanMessageTiny(msg, TRUE, 0x0, 8);
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		REQUIRE( tiny.getLastUpdateTime() == arbitraryUpdateTime );
	}

	SECTION( "update can tinybms battery voltage" ) {
		srand(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());

		for (int i = 0; i < 20; i++) {
			float testVoltage = (float)(rand() % 1000 - 500) / 100.0f;

			packFloatMessageTiny(msg, testVoltage, TINYBMS_PARAM_ID_BATTERY_VOLTAGE);
			canBusMock.setCanMessage(msg);

			// act
			Handler::instance().handle();

			// assert
			bool isValid = false;
			REQUIRE ( tiny.getBatteryVolt(isValid).toFloat() == Approx(testVoltage).margin(0.1));
			REQUIRE ( isValid );
		}

		testValidation(tiny, &CanSensorTinyBms::getBatteryVolt);
	}

	SECTION( "update can tinybms battery current" ) {
		srand(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());

		for (int i = 0; i < 20; i++) {
			float testCurrent = (float)(rand() % 1000 - 500) / 100.0f;

			packFloatMessageTiny(msg, testCurrent, TINYBMS_PARAM_ID_BATTERY_CURRENT);
			canBusMock.setCanMessage(msg);

			// act
			Handler::instance().handle();

			// assert
			bool isValid = false;
			REQUIRE ( tiny.getBatteryCurrent(isValid).toFloat() == Approx(-testCurrent).margin(0.1));
			REQUIRE ( isValid );
		}

		testValidation(tiny, &CanSensorTinyBms::getBatteryCurrent);
	}

	SECTION( "update can tinybms battery cell voltages" ) {
		srand(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());

		for (int i = 0; i < 20; i++) {
			float testCellVoltage1 = (float)(rand() % 200 - 100) / 100.0f;
			float testCellVoltage2 = (float)(rand() % 200 - 100) / 100.0f;
			float minVal = min(testCellVoltage1, testCellVoltage2);
			float maxVal = max(testCellVoltage1, testCellVoltage2);

			packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_MIN_CELL_VOLTAGE, 4);
			packInt16Tiny((int16_t)(minVal * TINYBMS_CELL_VOLTAGE_SCALING_FACTOR), msg.data);
			canBusMock.setCanMessage(msg);
			Handler::instance().handle();

			packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_MAX_CELL_VOLTAGE, 4);
			packInt16Tiny((int16_t)(maxVal * TINYBMS_CELL_VOLTAGE_SCALING_FACTOR), msg.data);
			canBusMock.setCanMessage(msg);
			Handler::instance().handle();

			// assert
			bool isValid = false;
			REQUIRE ( tiny.getMinVolt(isValid).toFloat() == Approx(minVal).margin(0.001));
			REQUIRE ( isValid );
			isValid = false;
			REQUIRE ( tiny.getMaxVolt(isValid).toFloat() == Approx(maxVal).margin(0.001));
			REQUIRE ( isValid );
		}

		testValidation(tiny, &CanSensorTinyBms::getMinVolt);
		testValidation(tiny, &CanSensorTinyBms::getMaxVolt);
	}

	SECTION( "update can tinybms status" ) {
		int i = 0;
		for (int status : Statuses) {
			packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_STATUS, 4);
			packInt16Tiny((int16_t)status, msg.data);
			canBusMock.setCanMessage(msg);

			// act
			Handler::instance().handle();

			// assert
			bool isValid = false;
			REQUIRE ( tiny.getStatusBms(isValid) == (CanSensorBms::BmsStatus)(i++) );
			REQUIRE ( isValid );
		}

		testValidation(tiny, &CanSensorTinyBms::getStatusBms);
	}

	SECTION( "update can tinybms soc" ) {
		srand(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());

		for (int i = 0; i < 20; i++) {
			int32_t testSoc = (int32_t)fabs(rand()) % 100000000;
			float expectedSoc = (float)testSoc / TINYBMS_SOC_SCALING_FACTOR;
			
			packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_SOC, 6);
			packInt32Tiny(testSoc, msg.data);
			canBusMock.setCanMessage(msg);

			// act
			Handler::instance().handle();

			// assert
			bool isValid = false;
			REQUIRE ( tiny.getSoc(isValid).toFloat() == Approx(expectedSoc).margin(0.1));
			REQUIRE ( isValid );
		}

		testValidation(tiny, &CanSensorTinyBms::getSoc);
	}

	SECTION( "update can tinybms internal temp" ) {
		for (int i = -20; i <= 100; i += 15) {
			int testTemp = i;

			packTempMessageTiny(msg, testTemp, TINYBMS_TEMP_ID_INTERNAL);
			canBusMock.setCanMessage(msg);

			// act
			Handler::instance().handle();

			// assert
			bool isValid = false;
			REQUIRE ( tiny.getTempBms(isValid) == testTemp);
			REQUIRE ( isValid );
			REQUIRE ( tiny.getMaxBatteryTemp() == 0 );
			REQUIRE ( tiny.getAvgBatteryTemp() == 0 );
			REQUIRE ( tiny.getMinBatteryTemp() == 0 );
		}

		testValidation(tiny, &CanSensorTinyBms::getTempBms);
	}

	SECTION( "update can tinybms battery temp" ) {
		int j = 0;

		for (int i = -10; i <= 140; i += 15) {
			int testTemp1 = i;
			int testTemp2 = 140 - (j++ * 15);

			// update battery temp 1
			packTempMessageTiny(msg, testTemp1, TINYBMS_TEMP_ID_BATTERY_1);
			canBusMock.setCanMessage(msg);
			Handler::instance().handle();

			// update battery temp 2
			packTempMessageTiny(msg, testTemp2, TINYBMS_TEMP_ID_BATTERY_2);
			canBusMock.setCanMessage(msg);
			Handler::instance().handle();

			// assert
			bool isValid = false;
			REQUIRE ( tiny.getMaxBatteryTemp(isValid) == max(testTemp1, testTemp2) );
			REQUIRE ( isValid );
			isValid = false;
			REQUIRE ( tiny.getAvgBatteryTemp(isValid) == (testTemp1 + testTemp2) / 2 );
			REQUIRE ( isValid );
			isValid = false;
			REQUIRE ( tiny.getMinBatteryTemp(isValid) == min(testTemp1, testTemp2) );
			REQUIRE ( isValid );
			REQUIRE ( tiny.getTempBms(isValid) == 0);
			REQUIRE_FALSE ( isValid );
		}

		testValidation(tiny, &CanSensorTinyBms::getMaxBatteryTemp);
		testValidation(tiny, &CanSensorTinyBms::getAvgBatteryTemp);
		testValidation(tiny, &CanSensorTinyBms::getMinBatteryTemp);
	}

	SECTION( "update can tinybms fault" ) {
		// update status to FAULT
		packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_STATUS, 4);
		packInt16Tiny(0xFF, msg.data);
		canBusMock.setCanMessage(msg);

		setMillis(DEFAULT_START_TIME_MILLIS - TEST_REQUEST_INTERVAL);
		Handler::instance().handle();

		bool faultRequestSent = false;
		canBusMock.setSendMsgBuffer([&faultRequestSent](unsigned long id, byte ext, byte len, const byte *buf) {
			if (buf[0] == TINYBMS_PARAM_ID_EVENTS) {
				faultRequestSent = true;
			}
		});

		setMillis(DEFAULT_START_TIME_MILLIS);
		Handler::instance().handle();
		
		REQUIRE( faultRequestSent );

		// Verify that TinyBms handles all valid fault codes
		for (auto pair : CanSensorTinyBms::FaultCodeMap) {
			uint8_t fault = pair.first;
			BmsFault::Code expected = pair.second;

			packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_EVENTS, 8);
			msg.data[6] = fault;
			msg.data[7] = 1;
			canBusMock.setCanMessage(msg);

			// act
			Handler::instance().handle();

			// assert
			bool isValid = false;
			BmsFault::Code actual = (BmsFault::Code)tiny.getFault(isValid);
			REQUIRE ( actual == expected );
			REQUIRE ( isValid );
		}

		testValidation(tiny, &CanSensorTinyBms::getFault);

		// Verify that TinyBms handles invalid fault code
		setMillis(DEFAULT_START_TIME_MILLIS);

		uint8_t fault = 0x7;
		BmsFault::Code expected = BmsFault::NONE;

		packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_EVENTS, 8);
		msg.data[6] = fault;
		msg.data[7] = 1;
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// assert
		bool isValid = false;
		REQUIRE ( tiny.getFault(isValid) == expected );
		REQUIRE ( isValid );
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

void packCanMessageTiny(CanMessage& msg, uint8_t status, uint8_t id, uint8_t len) {
	msg.id = CAN_TINYBMS_RESPONSE;
	msg.data[TINYBMS_RSP_STATUS_BYTE] = status;
	msg.data[TINYBMS_RSP_PARAM_ID_BYTE] = id;
	msg.dataLength = len;
}

void packTempMessageTiny(CanMessage& msg, int16_t temp, uint8_t tempId) {
	packCanMessageTiny(msg, TRUE, TINYBMS_PARAM_ID_TEMP, TINYBMS_REQ_DATA_LENGTH - 2);
	msg.data[TINYBMS_TEMP_ID_BYTE] = tempId;
	packInt16Tiny(temp * TINYBMS_TEMP_SCALING_FACTOR, msg.data + 1);
}

void packFloatMessageTiny(CanMessage& msg, float val, uint8_t id) {
	packCanMessageTiny(msg, TRUE, id, TINYBMS_REQ_DATA_LENGTH - 2);
	packFloatTiny(val, msg.data);
}