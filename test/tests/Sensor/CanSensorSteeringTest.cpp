#include "settings.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorSteering.h"

TEST_CASE( "CanSensorSteering constructor test", "[CanSensorSteering]" ) {
	CanBusMock mock(1);
	CanInterface interface(&mock);
	CanSensorSteering steering(interface);

	REQUIRE( mock.messageAvail() == 5 );
}