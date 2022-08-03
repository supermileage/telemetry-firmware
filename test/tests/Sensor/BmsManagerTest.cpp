#include "test_config.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorTinyBms.h"
#include "CanSensorOrionBms.h"
#include "BmsManager.h"

#define FAKE_MESSAGE_AVAIL 5

TEST_CASE( "BmsManager::setCurrentBms", "[BmsManager]" ) {
	CanBusMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorTinyBms tiny(interface, 500);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, &tiny, BmsManager::Tiny);
	bool valid;

	setMillis(0);

	REQUIRE(bms == &tiny);
	REQUIRE(manager.getCurrentBms(valid) == BmsManager::Tiny);
	REQUIRE(manager.getCurrentBmsName().equals(tiny.getHumanName()));

	manager.setBms(BmsManager::Orion);

	REQUIRE(bms == &orion);
	REQUIRE(manager.getCurrentBms(valid) == BmsManager::Orion);
	REQUIRE(manager.getCurrentBmsName().equals(orion.getHumanName()));
}

TEST_CASE( "BmsManager::getCurrentBms", "[BmsManager]" ) {
	CanBusMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorTinyBms tiny(interface, 500);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, &tiny, BmsManager::Orion);
	bool valid;

	// begin all handleable objects
	Handler::instance().begin();

	SECTION( "BmsManager returns correct bms type before deselect timeout" ) {
		setMillis(0);
		CHECK( orion.getLastUpdateTime() == 0 );
		REQUIRE( tiny.getLastUpdateTime() == 0 );
		REQUIRE ( manager.getCurrentBms(valid) == BmsManager::Orion );

		setMillis( BmsManager::MillisecondsBeforeDeselect - 1 );
		REQUIRE ( manager.getCurrentBms(valid) == BmsManager::Orion );
	}

	SECTION( "BmsManager returns None after deselect timeout" ) {
		setMillis( BmsManager::MillisecondsBeforeDeselect );
		REQUIRE (manager.getCurrentBms(valid) == BmsManager::None );
	}
}

TEST_CASE( "BmsManager::handle", "[BmsManager][Handle]" ) {
	CanBusMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorTinyBms tiny(interface, 500);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, &tiny, BmsManager::Orion);
	bool valid;

	// begin all handleable objects
	Handler::instance().begin();

	// set up canBusMock to return can message
	canBusMock.setReadInterruptPin([]() { return false; });
	canBusMock.setCheckReceive([]() { return FAKE_MESSAGE_AVAIL; });

	SECTION( "1 off test case: BmsManager won't update current bms" ) {
		setMillis( BmsManager::UpdateInterval );

		// set up canBusMock to update TinyBms
		canBusMock.setGetCanId([]() { return CAN_TINYBMS_RESPONSE; });

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( tiny.getLastUpdateTime() == BmsManager::UpdateInterval );
		REQUIRE( orion.getLastUpdateTime() == 0 );
		REQUIRE( manager.getCurrentBms(valid) == BmsManager::Orion );
	}

	SECTION( "BmsManager changes selection to TinyBms if it has been updated more recently" ) {
		setMillis( BmsManager::UpdateInterval + 1 );

		// set up canBusMock to update TinyBms
		canBusMock.setGetCanId([]() { return CAN_TINYBMS_RESPONSE; });

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( tiny.getLastUpdateTime() == BmsManager::UpdateInterval + 1 );
		REQUIRE( orion.getLastUpdateTime() == 0 );
		REQUIRE( manager.getCurrentBms(valid) == BmsManager::Tiny );
	}

	SECTION( "BmsManager changes selection to OrionBms if it has been updated more recently" ) {
		setMillis( BmsManager::UpdateInterval + 1 );

		// set up canBusMock to update TinyBms
		canBusMock.setGetCanId([]() { return CAN_ORIONBMS_CELL; });
		canBusMock.setReadMsgBuffer([](byte* len, byte* buf) -> byte {
			*len = 8;
			return 0;
		});

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( tiny.getLastUpdateTime() == 0 );
		REQUIRE( orion.getLastUpdateTime() == BmsManager::UpdateInterval + 1 );
		REQUIRE( manager.getCurrentBms(valid) == BmsManager::Orion );
	}
}