#include "test_config.h"
#include <unordered_map>

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorAccessories.h"

#define NUM_IDS 7

/* Helper Functions */
void testValidation(CanSensorAccessories& accessories, const uint8_t statusId);

// status ids mapped to their getters 
std::unordered_map<uint8_t, int (CanSensorAccessories::*)(bool&)> statusIdToGetter = {
	{ CanSensorAccessories::StatusIdHeadlights, &CanSensorAccessories::getStatusHeadlights },
	{ CanSensorAccessories::StatusIdBrakelights, &CanSensorAccessories::getStatusBrakelights },
	{ CanSensorAccessories::StatusIdHorn, &CanSensorAccessories::getStatusHorn },
	{ CanSensorAccessories::StatusIdHazards, &CanSensorAccessories::getStatusHazards },
	{ CanSensorAccessories::StatusIdRightSignal, &CanSensorAccessories::getStatusRightSignal },
	{ CanSensorAccessories::StatusIdLeftSignal, &CanSensorAccessories::getStatusLeftSignal },
	{ CanSensorAccessories::StatusIdWipers, &CanSensorAccessories::getStatusWipers }
};

/* Tests */
TEST_CASE( "CanSensorAccessories::getHumanName test", "[CanSensorAccessories][Sensor]" ) {
	CanInterface interface(nullptr);
	CanSensorAccessories accessories(interface, CAN_ACC_STATUS);

	REQUIRE (accessories.getHumanName() == "CanSensorAccessories" );
}

TEST_CASE( "CanSensorAccessories getters test -- all status properties unknown on startup", "[CanSensorAccessories][Sensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorAccessories accessories(interface, CAN_ACC_STATUS);

	Handler::instance().begin();

	for (const auto& pair : statusIdToGetter) {
		bool statusIsValid = false;
		REQUIRE ( (accessories.*pair.second)(statusIsValid) == CanSensorAccessories::Unknown );
		REQUIRE_FALSE ( statusIsValid );
	}
}

TEST_CASE( "CanSensorAccessories::update -- test update individual properties", "[CanSensorAccessories][Sensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorAccessories accessories(interface, CAN_ACC_STATUS);
	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_ACC_STATUS;
	msg.dataLength = 1;

	Handler::instance().begin();

	SECTION("Test update headlights status") {
		msg.data[0] = (CanSensorAccessories::StatusIdHeadlights << 1) | 0x1;
		canBusMock.setCanMessage(msg);
		
		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusHeadlights(statusIsValid) );
		REQUIRE( statusIsValid );

		testValidation(accessories, CanSensorAccessories::StatusIdHeadlights);
	}

	SECTION("Test update brakelights status") {
		msg.data[0] = (CanSensorAccessories::StatusIdBrakelights << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusBrakelights(statusIsValid) );
		REQUIRE( statusIsValid );

		testValidation(accessories, CanSensorAccessories::StatusIdBrakelights);
	}

	SECTION("Test update horn status") {
		msg.data[0] = (CanSensorAccessories::StatusIdHorn << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusHorn(statusIsValid) );
		REQUIRE( statusIsValid );

		testValidation(accessories, CanSensorAccessories::StatusIdHorn);
	}

	SECTION("Test update hazards status") {
		msg.data[0] = (CanSensorAccessories::StatusIdHazards << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusHazards(statusIsValid) );
		REQUIRE( statusIsValid );

		testValidation(accessories, CanSensorAccessories::StatusIdHazards);
	}

	SECTION("Test update right signal status") {
		msg.data[0] = (CanSensorAccessories::StatusIdRightSignal << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusRightSignal(statusIsValid) );
		REQUIRE( statusIsValid );

		testValidation(accessories, CanSensorAccessories::StatusIdRightSignal);
	}

	SECTION("Test update left signal status") {
		msg.data[0] = (CanSensorAccessories::StatusIdLeftSignal << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusLeftSignal(statusIsValid) == CanSensorAccessories::On );
		REQUIRE( statusIsValid );

		testValidation(accessories, CanSensorAccessories::StatusIdLeftSignal);
	}

	SECTION("Test update wipers status") {
		msg.data[0] = (CanSensorAccessories::StatusIdWipers << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusWipers(statusIsValid) );
		REQUIRE( statusIsValid );

		testValidation(accessories, CanSensorAccessories::StatusIdWipers);
	}
}

TEST_CASE("CanSensorAccessories::update -- full buffer test -- on then off", "[CanSensorAccessories][Sensor]") {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorAccessories accessories(interface, CAN_ACC_STATUS);
	setMillis(DEFAULT_START_TIME_MILLIS);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_ACC_STATUS;
	msg.dataLength = NUM_IDS;

	Handler::instance().begin();

	// this test will have to be restructure if we have more statuses than can fit in a single CanMessage data buffer
	REQUIRE ( statusIdToGetter.size() <= 8 );

	// set all statuses to 'on'
	int i = 0;
	for (const auto& pair : statusIdToGetter)
		msg.data[i++] = (pair.first << 1) | 0x1;
	canBusMock.setCanMessage(msg);

	// act
	Handler::instance().handle();

	// assert
	for  (const auto& pair : statusIdToGetter) {
		bool statusIsValid = false;
		REQUIRE( (accessories.*pair.second)(statusIsValid) );
		REQUIRE( statusIsValid );
	}

	// set all statuses to 'off'
	i = 0;
	for (const auto& pair : statusIdToGetter)
		msg.data[i++] = (pair.first << 1) | 0x0;
	canBusMock.setCanMessage(msg);

	// act
	Handler::instance().handle();

	// assert
	for  (const auto pair : statusIdToGetter) {
		bool statusIsValid = false;
		REQUIRE_FALSE( (accessories.*pair.second)(statusIsValid) );
		REQUIRE( statusIsValid );
	}
}

// Checks validation edge cases
void testValidation(CanSensorAccessories& accessories, const uint8_t statusId) {
	bool statusIsValid = false;
	setMillis(DEFAULT_STALE_TIME_MILLIS - 1);
	(accessories.*statusIdToGetter[statusId])(statusIsValid);
	REQUIRE ( statusIsValid );

	setMillis(DEFAULT_STALE_TIME_MILLIS);
	(accessories.*statusIdToGetter[statusId])(statusIsValid);
	REQUIRE_FALSE ( statusIsValid );
}