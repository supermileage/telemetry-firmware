#include <iostream>

#include "test_config.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorOrionBms.h"

/* Helper Functions */
void packBatteryData(float voltage, float current, float soc, uint8_t* buf);
void packCellData(float cellVoltageLow, float cellVoltageHigh, float cellVoltageAvg, uint8_t* buf);
void packTempData(int8_t packTempLow, int8_t packTempHigh, int8_t packTempAvg, int8_t bmsTemp, uint8_t* buf);
void packInt16(int16_t val, uint8_t* buf);

// orion faults ordered byte 1 - 3, from least to most significant bit
// ie. data[1], data[2], data[3], each byte right to left
const int orderedFaults[] = {
	HARDWARE, CURRENT_SENSOR_DISCONNECT, OPEN_WIRING, CURRENT_CHARGE_HIGH, CURRENT_DISCHARGE_HIGH,
	VOLTAGE_HIGH, CELL_OVER_5V, TEMP_HIGH, VOLTAGE_LOW, COMMUNICATION_CAN,THERMISTOR_INTERNAL,
	THERMISTOR_PACK, BALANCING, CELL_BANK, COMMUNICATION_INTERNAL, INTERNAL_LOGIC, WEAK_CELL, WEAK_PACK,
	TEMP_LOW
};

/* Tests */
TEST_CASE( "CanSensorOrionBms::getHumanName", "[CanSensorOrionBms][Sensor]" ) {
	CanInterface interface(nullptr);
	CanSensorOrionBms orion(interface);

	REQUIRE( orion.getHumanName().equals("CanSensorOrionBms") );
}

TEST_CASE( "CanSensorOrionBms::update CAN_ORIONBMS_STATUS", "[CanSensorOrionBms][Sensor][CanSensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	setMillis(0);

	Handler::instance().begin();

	SECTION( "Test CanSensorOrionBms::update charge status" ) {
		// Discharge
		// set up discharge enabled msg
		CanMessage msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_STATUS;
		msg.dataLength = 4;
		msg.data[0] = 0x1;
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// assert
		bool statusValid = false;

		REQUIRE( orion.getStatusBms(statusValid) == CanSensorBms::DischargeEnabled );
		REQUIRE( statusValid );

		// Charge
		// set up charge enabled msg
		msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_STATUS;
		msg.dataLength = 4;
		msg.data[0] = 0x2;
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		REQUIRE( orion.getStatusBms(statusValid) == CanSensorBms::ChargeEnabled );
		REQUIRE( statusValid );

		// test validation
		setMillis( STALE_INTERVAL );
		
		REQUIRE( orion.getStatusBms(statusValid) == CanSensorBms::ChargeEnabled );
		REQUIRE( !statusValid );
	}

	SECTION( "Test CanSensorOrionBms::update fault -- _parseFault() returns correct fault for each flag" ) {
		CanMessage msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_STATUS;
		msg.dataLength = 4;
		msg.data[0] = 0x0;
		canBusMock.setCanMessage(msg);

		int faultIndex = 0;
		
		for (int i = 1; i < 4; i++) {
			int bitCount = 8;
			if (i == 3)
				bitCount = 3;

			// set all 
			msg.data[1] = 0x0; msg.data[2] = 0x0; msg.data[3] = 0x0;

			for (int j = 0; j < bitCount; j++) {
				// set up
				msg.data[i] = 0xFF & (0x1 << j);
				canBusMock.setCanMessage(msg);

				// act
				Handler::instance().handle();

				// assert
				bool faultIsValid = false;
				REQUIRE ( orion.getFault(faultIsValid) == orderedFaults[faultIndex++] );
				REQUIRE ( faultIsValid );
			}
		}

		// test validation
		setMillis( STALE_INTERVAL );
		
		bool faultIsValid = true;
		orion.getFault(faultIsValid);
		REQUIRE ( !faultIsValid );
	} // SECTION

	SECTION( "Test CanSensorOrionBms::update fault -- mixed flags: highest priority (lowest value) is always returned" ) {
		CanMessage msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_STATUS;
		msg.dataLength = 4;
		msg.data[0] = 0x0;
		canBusMock.setCanMessage(msg);

		int faultIndex = 0;

		for (int i = 1; i < 4; i++) {
			int bitCount = 8;
			if (i == 3)
				bitCount = 3;

			// set all lower priority flags high
			msg.data[1] = 0x0; msg.data[2] = 0x0; msg.data[3] = 0x0;

			for (int j = 0; j < bitCount; j++) {
				// set up
				for (int k = 0; k <= j; k++)
					msg.data[i] = 0xFF & (0x1 << k);
				for (int k = i + 1; k < 4; k++)
					msg.data[k] = 0xFF;
				canBusMock.setCanMessage(msg);

				// act
				Handler::instance().handle();

				// assert
				bool faultIsValid = false;
				REQUIRE ( orion.getFault(faultIsValid) == orderedFaults[faultIndex++] );
				REQUIRE ( faultIsValid );
			}
		}
	} // SECTION
}

TEST_CASE("Test CanSensorOrionBms::update CAN_ORIONBMS_PACK", "[CanSensorOrionBms][Sensor][CanSensor]") {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	setMillis(0);

	Handler::instance().begin();

	SECTION("Test CanSensorOrionBms::update battery voltage") {
		// set up pack message with battery voltage value
		CanMessage msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_PACK;
		msg.dataLength = 5;
		packBatteryData(32, 0, 0, msg.data);
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// assert
		bool batteryVoltageIsValid = false;
		REQUIRE ( orion.getBatteryVolt(batteryVoltageIsValid) == "32.0" );
		REQUIRE ( batteryVoltageIsValid );
		REQUIRE ( orion.getBatteryCurrent() == "0.0" );
		REQUIRE ( orion.getSoc() == "0.0" );

		// test validation
		setMillis( STALE_INTERVAL );
		
		orion.getBatteryCurrent(batteryVoltageIsValid);
		REQUIRE ( !batteryVoltageIsValid );
	}

	SECTION("Test CanSensorOrionBms::update battery current") {
		// set up pack message with battery voltage value
		CanMessage msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_PACK;
		msg.dataLength = 5;
		packBatteryData(0, 32, 0, msg.data);
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// assert
		bool batteryCurrentIsValid = false;
		REQUIRE ( orion.getBatteryCurrent(batteryCurrentIsValid) == "32.0" );
		REQUIRE ( batteryCurrentIsValid );
		REQUIRE ( orion.getBatteryVolt() == "0.0" );
		REQUIRE ( orion.getSoc() == "0.0" );

		// test validation
		setMillis( STALE_INTERVAL );
		
		orion.getBatteryCurrent(batteryCurrentIsValid);
		REQUIRE ( !batteryCurrentIsValid );
	}

	SECTION("Test CanSensorOrionBms::update soc") {
		// set up pack message with battery voltage value
		CanMessage msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_PACK;
		msg.dataLength = 5;
		packBatteryData(0, 0, 64, msg.data);
		canBusMock.setCanMessage(msg);

		// act
		Handler::instance().handle();

		// asserts
		bool socIsValid = false;
		REQUIRE ( orion.getSoc(socIsValid) == "64.0" );
		REQUIRE ( socIsValid );
		REQUIRE ( orion.getBatteryVolt() == "0.0" );
		REQUIRE ( orion.getBatteryCurrent() == "0.0" );

		// test validation
		setMillis( STALE_INTERVAL );
		
		orion.getSoc(socIsValid);
		REQUIRE ( !socIsValid );
	}

	SECTION("Test CanSensorOrionBms::update pack data -- different values in possible ranges") {
		CanMessage msg = CAN_MESSAGE_NULL;
		msg.id = CAN_ORIONBMS_PACK;
		msg.dataLength = 5;

		for (int i = 0; i < 10; i++) {
			// set up pack values with random decimal offset value:
			// voltage ranges from -50 to 50V
			// current ranges from -30 to 30A
			// soc ranges from 0 to 100%
			float voltage = (float)(i * 10 - 50) + (float)rand() / RAND_MAX;
			float current = (float)(i * 8 - 40) + (float)rand() / RAND_MAX;
			float soc = i * 10 + (float)rand() / RAND_MAX;
			packBatteryData(voltage, current, soc, msg.data);
			canBusMock.setCanMessage(msg);

			// act
			Handler::instance().handle();

			// assert
			REQUIRE ( orion.getBatteryVolt().toFloat() == Approx(voltage).margin(0.1) );
			REQUIRE ( orion.getBatteryCurrent().toFloat() == Approx(current).margin(0.1) );
			REQUIRE ( orion.getSoc().toFloat() == Approx(soc).margin(0.5) );
		}
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
	buf[4] = bmsTemp;
}

void packInt16(int16_t val, uint8_t* buf) {
	*buf = val >> 8;
	*(buf + 1) = val & 0xFF;
}