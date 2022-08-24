#include "test_config.h"

#include "SensorEcu.h"
#include "TelemetrySerialMock.h"

TEST_CASE( "SensorEcu::getHumanName test", "[SensorEcu]" ) {
	SensorEcu ecu(nullptr);
	
	REQUIRE( ecu.getHumanName() == "ECU" );
}