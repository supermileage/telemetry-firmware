#include "settings.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorTinyBms.h"
#include "CanSensorOrionBms.h"
#include "BmsManager.h"

#define FAKE_MESSAGE_AVAIL 5

TEST_CASE( "BmsManager::getCurrentBms test", "[BmsManager]" ) {
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
		REQUIRE( orion.getLastUpdateTime() == 0 );
		REQUIRE( tiny.getLastUpdateTime() == 0 );
		REQUIRE ( manager.getCurrentBms(valid) == BmsManager::Orion );

		setMillis( BmsManager::MillisecondsBeforeDeselect - 1 );
		REQUIRE (manager.getCurrentBms(valid) == BmsManager::Orion );
	}

	SECTION( "BmsManager returns None after deselect timeout" ) {
		setMillis( BmsManager::MillisecondsBeforeDeselect );
		REQUIRE (manager.getCurrentBms(valid) == BmsManager::None );
	}

	SECTION( "BmsManager changes selection if different Bms has been updated more recently" ) {
		setMillis( BmsManager::UpdateInterval + 1 );

		// set up canBusMock to update TinyBms
		canBusMock.setReadInterruptPin([]() { return false; });
		canBusMock.setGetCanId([]() { return CAN_TINYBMS_RESPONSE; });
		canBusMock.setCheckReceive([]() { return FAKE_MESSAGE_AVAIL; });

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( tiny.getLastUpdateTime() == BmsManager::UpdateInterval + 1 );
		REQUIRE( orion.getLastUpdateTime() == 0 );
		REQUIRE( manager.getCurrentBms(valid) == BmsManager::Tiny );
	}
}