#include "test_config.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorAccessories.h"

TEST_CASE( "CanSensorAccessories constructor test", "[CanSensorAccessories]" ) {
	CanBusMock mock(1);
	CanInterface interface(&mock);
	CanSensorAccessories accessories(interface, 0);

	REQUIRE( mock.messageAvail() == 3 );
}