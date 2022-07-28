#include "settings.h"

#include "CanInterface.h"
#include "CanBusMock.h"
#include "CanSensorTinyBms.h"

TEST_CASE( "CanSensorTinyBms constructor test", "[CanSensorTinyBms]" ) {
	CanBusMock mock(1);
	CanInterface interface(&mock);
	CanSensorTinyBms bms(interface, 500);

	REQUIRE( mock.messageAvail() == 6 );
}