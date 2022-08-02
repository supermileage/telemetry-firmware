#include "test_config.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorOrionBms.h"

TEST_CASE( "CanSensorOrionBms constructor test", "[CanSensorOrionBms]" ) {
	CanBusMock mock(1);
	CanInterface interface(&mock);
	CanSensorOrionBms bms(interface);

	REQUIRE( mock.messageAvail() == 1 );
}