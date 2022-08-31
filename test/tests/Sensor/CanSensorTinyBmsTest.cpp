#include <iostream>

#include "test_config.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorTinyBms.h"

/* Helper Functions */

template <typename T>
void testValidation(CanSensorTinyBms& bms, T (CanSensorTinyBms::*getter)(bool&)) {
	bool statusIsValid = false;
	setMillis(DEFAULT_STALE_TIME_MILLIS - 1);
	(bms.*getter)(statusIsValid);
	REQUIRE ( statusIsValid );

	setMillis(DEFAULT_STALE_TIME_MILLIS);
	(bms.*getter)(statusIsValid);
	REQUIRE_FALSE ( statusIsValid );
}

// Ordered Faults unlikely but here anyway
const int orderedFaults[] = {};

/* Tests */
TEST_CASE( "CanSensorTinyBms::getHumanName", "[CanSensorTinyBms][Sensor]" ) {
	CanInterface interface(nullptr);
	CanSensorTinyBms bms(interface);

	REQUIRE( bms.getHumanName().equals("CanSensorTinyBms") );
}

TEST_CASE( "CanSensorTinyBms::update CAN_TINYBMS_REQUEST", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms bms(interface);

	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_TINYBMS_REQUEST;
	msg.dataLength = TINYBMS_REQ_DATA_LENGTH;

	Handler::instance().begin();

	SECTION( "YO" ) {

		testValidation(bms, &CanSensorTinyBms::getSOMETHING);
	}

	REQUIRE( mock.messageAvail() == 1 );
}

TEST_CASE( "CanSensorTinyBms::update CAN_TINYBMS_RESPONSE", "[CanSensorTinyBms][Sensor][CanSensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorTinyBms bms(interface);

	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_TINYBMS_RESPONSE;
	msg.dataLength = TINYBMS_REQ_DATA_LENGTH;

	Handler::instance().begin();

	SECTION( "YO" ) {

		testValidation(bms, &CanSensorTinyBms::getSOMETHING);
	}

	REQUIRE( mock.messageAvail() == 1 );
}

/* Helper Function Definitions */