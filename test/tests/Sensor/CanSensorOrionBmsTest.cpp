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

TEST_CASE("Test CanSensorOrionBms::update CAN_ORIONBMS_PACK") {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	setMillis(0);

	Handler::instance().begin();


}

/* Helper Function Definitions */
void packBatteryData(float voltage, float current, float soc, uint8_t* buf) {
	packInt16((int16_t)voltage * 10, buf);
	packInt16((int16_t)voltage * 10, buf + 2);
	buf[4] = (uint8_t)soc * 2;
}

void packCellData(float cellVoltageLow, float cellVoltageHigh, float cellVoltageAvg, uint8_t* buf) {
	packInt16((int16_t)cellVoltageLow * 1000, buf);
	packInt16((int16_t)cellVoltageHigh * 1000, buf + 2);
	packInt16((int16_t)cellVoltageAvg * 1000, buf + 4);
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