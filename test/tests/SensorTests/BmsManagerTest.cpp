#include "test_config.h"

#include "CanInterface.h"
#include "CanControllerMock.h"
#include "CanSensorOrionBms.h"
#include "BmsManager.h"

#define FAKE_MESSAGE_AVAIL 5

TEST_CASE( "BmsManager::setCurrentBms", "[BmsManager]" ) {
	CanControllerMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, BmsManager::None);

	setMillis(0);

	REQUIRE(bms != &orion);
	REQUIRE(manager.getCurrentBms() == BmsManager::None);
	REQUIRE(manager.getCurrentBmsName().equals("None"));

	manager.setBms(BmsManager::Orion);

	REQUIRE(bms == &orion);
	REQUIRE(manager.getCurrentBms() == BmsManager::Orion);
	REQUIRE(manager.getCurrentBmsName().equals(orion.getHumanName()));
}

TEST_CASE( "BmsManager::getCurrentBms", "[BmsManager]" ) {
	CanControllerMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, BmsManager::Orion);

	// begin all handleable objects
	Handler::instance().begin();

	SECTION( "BmsManager returns correct bms type before deselect timeout" ) {
		setMillis(0);
		CHECK( orion.getLastUpdateTime() == 0 );
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
	CanControllerMock canBusMock(FAKE_MESSAGE_AVAIL);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms orion(interface);
	CanSensorBms* bms;
	BmsManager manager(&bms, &orion, BmsManager::Orion);

	CanMessage msg = CAN_MESSAGE_NULL;
	msg.id = CAN_ORIONBMS_CELL;
	msg.dataLength = 8;

	// begin all handleable objects
	Handler::instance().begin();

	SECTION( "BmsManager changes selection to OrionBms if it has been updated more recently" ) {
		// set up canBusMock to update OrionBms
		canBusMock.setCanMessage(msg);
		setMillis( BmsManager::UpdateInterval + 1 );

		// handle all handleables so they update
		Handler::instance().handle();
		
		REQUIRE( orion.getLastUpdateTime() == BmsManager::UpdateInterval + 1 );
		REQUIRE( manager.getCurrentBms() == BmsManager::Orion );
	}
}