#include "test_config.h"

#include <functional>
#include <math.h>

#include "SensorAccelerometer.h"
#include "AccelerometerControllerMock.h"

/* Helper Functions */

/* Tests */
TEST_CASE( "SensorAccelerometer::getHumanName test", "[SensorAccelerometer][Sensor]" ) {
    SensorAccelerometer accel(nullptr);
    
    REQUIRE( accel.getHumanName() == "Accelerometer" );
}

TEST_CASE( "SensorAccelerometer::begin -- pitch", "[SensorAccelerometer][Sensor][begin]" ) {
    AccelerometerControllerMock mock;
    SensorAccelerometer accel(&mock);

    SECTION("AccelerometerController fails to inititialize") {
        mock.setInit([]() { return false; });
        accel.begin();

        REQUIRE( accel.getInitStatus() == "Failure" );
    }

    SECTION("Accelerometer is perfectly level -- pitch is 0") {
        mock.setReturnValues(Vec3 { 0, -GRAVITY, 0 }, Vec3 { 0, 0, 0}, true);
        accel.begin();
        
        REQUIRE( accel.getIncline() == 0 );
    }

    SECTION("Accelerometer is tilted back 90 degrees over x") {
        mock.setReturnValues(Vec3 { 0, 0, GRAVITY }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline() == Approx(3.14 / 2).margin(0.01));
    }

    SECTION("Accelerometer is tilted forward 90 degrees over x") {
        mock.setReturnValues(Vec3 { 0, 0, -GRAVITY }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline() == Approx(-3.14 / 2).margin(0.01));
    }

    SECTION("Accelerometer is tilted back 45 degrees over x") {
        mock.setReturnValues(Vec3 { 0, (float)((-GRAVITY)/sqrt(2)), (float)((GRAVITY)/sqrt(2)) }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline() == Approx(3.14 / 4).margin(0.01));
    }

    SECTION("Accelerometer is tilted forward 45 degrees over x") {
        mock.setReturnValues(Vec3 { 0, (float)((-GRAVITY)/sqrt(2)), (float)((-GRAVITY)/sqrt(2)) }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline() == Approx(-3.14 / 4).margin(0.01));
    }
}