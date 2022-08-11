#include "test_config.h"
#include <array>

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorAccessories.h"

#define NUM_IDS 7

const uint8_t orderedStatusIds[] {
    CanSensorAccessories::StatusIdHeadlights,
    CanSensorAccessories::StatusIdBrakelights,
    CanSensorAccessories::StatusIdHorn,
    CanSensorAccessories::StatusIdHazards,
    CanSensorAccessories::StatusIdRightSignal,
    CanSensorAccessories::StatusIdLeftSignal,
	CanSensorAccessories::StatusIdWipers
};

// array of all status getters (in same orders as orderedStatusIds)
std::array<int (CanSensorAccessories::*)(bool&), NUM_IDS> orderedGetters {
    &CanSensorAccessories::getStatusHeadlights,
    &CanSensorAccessories::getStatusBrakelights,
    &CanSensorAccessories::getStatusHorn,
    &CanSensorAccessories::getStatusHazards,
    &CanSensorAccessories::getStatusRightSignal,
    &CanSensorAccessories::getStatusLeftSignal,
	&CanSensorAccessories::getStatusWipers
};

TEST_CASE( "CanSensorAccessories::getHumanName test", "[CanSensorAccessories][Sensor]" ) {
	CanInterface interface(nullptr);
	CanSensorAccessories accessories(interface, CAN_ACC_STATUS);

	REQUIRE (accessories.getHumanName() == "CanSensorAccessories" );
}

TEST_CASE( "CanSensorAccessories::update -- test update individual properties", "[CanSensorAccessories][Sensor]" ) {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorAccessories accessories(interface, CAN_ACC_STATUS);
	setMillis(0);

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

		// test validation
		setMillis( STALE_INTERVAL );
		accessories.getStatusHeadlights(statusIsValid);
		REQUIRE( !statusIsValid );
	}

	SECTION("Test update brakelights status") {
		msg.data[0] = (CanSensorAccessories::StatusIdBrakelights << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusBrakelights(statusIsValid) );
		REQUIRE( statusIsValid );

		// test validation
		setMillis( STALE_INTERVAL );
		accessories.getStatusBrakelights(statusIsValid);
		REQUIRE( !statusIsValid );
	}

	SECTION("Test update horn status") {
		msg.data[0] = (CanSensorAccessories::StatusIdHorn << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusHorn(statusIsValid) );
		REQUIRE( statusIsValid );

		// test validation
		setMillis( STALE_INTERVAL );
		accessories.getStatusHorn(statusIsValid);
		REQUIRE( !statusIsValid );
	}

	SECTION("Test update hazards status") {
		msg.data[0] = (CanSensorAccessories::StatusIdHazards << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusHazards(statusIsValid) );
		REQUIRE( statusIsValid );

		// test validation
		setMillis( STALE_INTERVAL );
		accessories.getStatusHazards(statusIsValid);
		REQUIRE( !statusIsValid );
	}

	SECTION("Test update right signal status") {
		msg.data[0] = (CanSensorAccessories::StatusIdRightSignal << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusRightSignal(statusIsValid) );
		REQUIRE( statusIsValid );

		// test validation
		setMillis( STALE_INTERVAL );
		accessories.getStatusRightSignal(statusIsValid);
		REQUIRE( !statusIsValid );
	}

	SECTION("Test update left signal status") {
		msg.data[0] = (CanSensorAccessories::StatusIdLeftSignal << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusLeftSignal(statusIsValid) );
		REQUIRE( statusIsValid );

		// test validation
		setMillis( STALE_INTERVAL );
		accessories.getStatusLeftSignal(statusIsValid);
		REQUIRE( !statusIsValid );
	}

	SECTION("Test update wipers status") {
		msg.data[0] = (CanSensorAccessories::StatusIdWipers << 1) | 0x1;
		canBusMock.setCanMessage(msg);

		Handler::instance().handle();

		bool statusIsValid = false;
		REQUIRE( accessories.getStatusWipers(statusIsValid) );
		REQUIRE( statusIsValid );

		// test validation
		setMillis( STALE_INTERVAL );
		accessories.getStatusWipers(statusIsValid);
		REQUIRE( !statusIsValid );
	}
}

TEST_CASE("CanSensorAccessories::update -- full buffer test", "[CanSensorAccessories][Sensor]") {
	CanBusMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorAccessories accessories(interface, CAN_ACC_STATUS);
	setMillis(0);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_ACC_STATUS;
	msg.dataLength = NUM_IDS;

	Handler::instance().begin();

	// setup
	for (uint8_t i = 0; i < NUM_IDS; i++)
		msg.data[i] = (orderedStatusIds[i] << 1) | 0x1;
	canBusMock.setCanMessage(msg);

	// act
	Handler::instance().handle();

	// assert
	for  (uint8_t i = 0; i < NUM_IDS; i++) {
		bool statusIsValid = false;
		REQUIRE( (accessories.*orderedGetters[i])(statusIsValid) );
		REQUIRE( statusIsValid );

	}
}