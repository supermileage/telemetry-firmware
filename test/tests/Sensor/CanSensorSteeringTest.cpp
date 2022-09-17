#include "test_config.h"

#include "CanInterface.h"
#include "CanControllerMock.h"
#include "CanSensorSteering.h"

TEST_CASE( "CanSensorSteering constructor test", "[CanSensorSteering]" ) {
	CanControllerMock mock(1);
	CanInterface interface(&mock);
	CanSensorSteering steering(interface);

	REQUIRE( mock.messageAvail() == 1 );
}