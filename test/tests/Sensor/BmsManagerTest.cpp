#include "settings.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorTinyBms.h"
#include "CanSensorOrionBms.h"
#include "BmsManager.h"

TEST_CASE( "BmsManager constructor test", "[BmsManager]" ) {
	CanBusMock mock(1);
	CanInterface interface(&mock);
	CanSensorOrionBms orion(interface);
	CanSensorTinyBms tiny(interface, 500);
	CanSensorBms* bms;
	BmsManager(&bms, &tiny, &orion, BmsManager::Orion);

	REQUIRE( mock.messageAvail() == 2 );
}