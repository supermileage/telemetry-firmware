#include "test_config.h"

#include "CanInterface.h"
#include "CanControllerMock.h"
#include "CanSensorTinyBms.h"

TEST_CASE( "CanSensorTinyBms constructor test", "[CanSensorTinyBms]" ) {
	CanControllerMock mock(1);
	CanInterface interface(&mock);
	CanSensorTinyBms bms(interface, 500);

	REQUIRE( mock.messageAvail() == 1 );
}