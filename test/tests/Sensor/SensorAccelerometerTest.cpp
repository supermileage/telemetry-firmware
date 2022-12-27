#include "test_config.h"

#include <functional>
#include <array>
#include <string.h>

#include "SensorAccelerometer.h"
#include "AccelerometerControllerMock.h"

/* Helper Functions */

/* Tests */
TEST_CASE( "SensorAccelerometer::getHumanName test", "[SensorAccelerometer][Sensor]" ) {
    SensorAccelerometer accel(nullptr);

    REQUIRE( accel.getHumanName() == "Accelerometer" );
}