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

	setMillis(0);

	REQUIRE(bms == &tiny);
	REQUIRE(manager.getCurrentBms() == BmsManager::Tiny);
	REQUIRE(manager.getCurrentBmsName().equals(tiny.getHumanName()));

	manager.setBms(BmsManager::Orion);

	REQUIRE(bms == &orion);
	REQUIRE(manager.getCurrentBms() == BmsManager::Orion);
	REQUIRE(manager.getCurrentBmsName().equals(orion.getHumanName()));
}

TEST_CASE( "BmsManager::getCurrentBms", "[BmsManager]" ) {
	CanBusMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorTinyBms tiny(interface, 500);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, &tiny, BmsManager::Orion);

	// begin all handleable objects
	Handler::instance().begin();

	SECTION( "BmsManager returns correct bms type before deselect timeout" ) {
		setMillis(0);
		CHECK( orion.getLastUpdateTime() == 0 );
		REQUIRE( tiny.getLastUpdateTime() == 0 );
		REQUIRE ( manager.getCurrentBms() == BmsManager::Orion );

		setMillis( BmsManager::MillisecondsBeforeDeselect - 1 );
		REQUIRE ( manager.getCurrentBms() == BmsManager::Orion );
	}

	SECTION( "BmsManager returns None after deselect timeout" ) {
		setMillis( BmsManager::MillisecondsBeforeDeselect );
		REQUIRE (manager.getCurrentBms() == BmsManager::None );
	}
}

TEST_CASE( "BmsManager::handle", "[BmsManager][Handle]" ) {
	CanBusMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorTinyBms tiny(interface, 500);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, &tiny, BmsManager::Orion);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_TINYBMS_RESPONSE;
	msg.dataLength = 1;

	// begin all handleable objects
	Handler::instance().begin();

	SECTION( "1-off test case: BmsManager won't update current bms" ) {
		// set up canBusMock to update TinyBms
		canBusMock.setCanMessage(msg);
		setMillis( BmsManager::UpdateInterval );

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( tiny.getLastUpdateTime() == BmsManager::UpdateInterval );
		REQUIRE( orion.getLastUpdateTime() == 0 );
		REQUIRE( manager.getCurrentBms() == BmsManager::Orion );
	}

	SECTION( "BmsManager changes selection to TinyBms if it has been updated more recently" ) {
		// set up canBusMock to update TinyBms
		canBusMock.setCanMessage(msg);
		setMillis( BmsManager::UpdateInterval + 1 );

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( tiny.getLastUpdateTime() == BmsManager::UpdateInterval + 1 );
		REQUIRE( orion.getLastUpdateTime() == 0 );
		REQUIRE( manager.getCurrentBms() == BmsManager::Tiny );
	}

	SECTION( "BmsManager changes selection to OrionBms if it has been updated more recently" ) {
		// set up canBusMock to update TinyBms
		msg.id = CAN_ORIONBMS_CELL;
		msg.dataLength = 8;
		canBusMock.setCanMessage(msg);
		setMillis( BmsManager::UpdateInterval + 1 );

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( tiny.getLastUpdateTime() == 0 );
		REQUIRE( orion.getLastUpdateTime() == BmsManager::UpdateInterval + 1 );
		REQUIRE( manager.getCurrentBms() == BmsManager::Orion );
	}
}