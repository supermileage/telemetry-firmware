#include <iostream>
#include "settings.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorTinyBms.h"
#include "CanSensorOrionBms.h"
#include "BmsManager.h"

TEST_CASE( "BmsManager constructor test", "[BmsManager]" ) {
	CanBusMock mock(0);
	CanInterface interface(&mock);
	CanSensorOrionBms orion(interface);
	CanSensorTinyBms tiny(interface, 500);
	CanSensorBms* bms;
	BmsManager manager(&bms, &tiny, &orion, BmsManager::Orion);

	REQUIRE( orion.getLastUpdateTime() == 0 );
	REQUIRE( tiny.getLastUpdateTime() == 0 );

	uint64_t time = millis();

	std::cout << "time  = " << time << std::endl;
}