#include "test_config.h"

#include <functional>
#include <math.h>

#include "SensorAccelerometer.h"
#include "AccelerometerControllerMock.h"

#define EULERS 2.7182f

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
        mock.setReturnValues(Vec3 { 0, ACCEL_GRAVITY, 0 }, Vec3 { 0, 0, 0}, true);
        accel.begin();
        
        REQUIRE( accel.getIncline().toFloat() == 0 );
    }

    SECTION("Accelerometer is tilted back 90 degrees over x") {
        mock.setReturnValues(Vec3 { 0, 0, ACCEL_GRAVITY }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline().toFloat() == Approx(3.14 / 2).margin(0.01));
    }

    SECTION("Accelerometer is tilted forward 90 degrees over x") {
        mock.setReturnValues(Vec3 { 0, 0, -ACCEL_GRAVITY }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline().toFloat() == Approx(-3.14 / 2).margin(0.01));
    }

    SECTION("Accelerometer is tilted back 45 degrees over x") {
        mock.setReturnValues(Vec3 { 0, (float)((ACCEL_GRAVITY)/sqrt(2)), (float)((ACCEL_GRAVITY)/sqrt(2)) }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline().toFloat() == Approx(3.14 / 4).margin(0.01));
    }

    SECTION("Accelerometer is tilted forward 45 degrees over x") {
        mock.setReturnValues(Vec3 { 0, (float)((ACCEL_GRAVITY)/sqrt(2)), (float)((-ACCEL_GRAVITY)/sqrt(2)) }, Vec3 { 0, 0, 0 }, true);
        accel.begin();
        
        REQUIRE( accel.getIncline().toFloat() == Approx(-3.14 / 4).margin(0.01));
    }
}

TEST_CASE( "SensorAccelerometer::handle", "[SensorAccelerometer][Sensor][handle]") {
    AccelerometerControllerMock mock;
    SensorAccelerometer accel(&mock);

    SECTION("Simulate accelerating and braking while going over bump") {
        float x = 0, z = 0;
        float y = ACCEL_GRAVITY;

        setMillis(0);
        mock.setReturnValues(Vec3 { x,y,z }, Vec3 { 0,0,0 }, true);
        accel.begin();

        // bump -- sharp normal curve from ~0 to 9.81 to ~0 -- peaks at 6 seconds
        std::function<float(float)> normalY = [](float x) {
            float median = 6;
            float variance = 0.5;
            return ACCEL_GRAVITY * pow(EULERS, (-0.5f * pow((x - median) / variance, 2)));
        };

        // forward -- smooth normal curve from ~0G to 0.2G to ~0G over 18 seconds
        std::function<float(float)> normalZ = [](float x) {
            float median = 9;
            float variance = 2;
            return ACCEL_GRAVITY * pow(EULERS, (-0.5f * pow((x - median) / variance, 2)));
        };

        // simulate accelerating forward while going over a bump
        // i = time in milliseconds
        for (uint i = 10; i <= 19000; i += ACCEL_READ_INTERVAL) {
            setMillis(i);
            y = normalY((float)i/1000);
            z = normalZ((float)i/1000);
            mock.setReturnValues(Vec3 { x, y + ACCEL_GRAVITY, z }, Vec3 { 0, 0, 0 }, true);

            accel.handle();

            float incline = accel.getIncline().toFloat();
            float haccel = accel.getHorizontalAcceleration().toFloat();
            float vaccel = accel.getVerticalAcceleration().toFloat();
            float actualY = accel.getAccel().y - ACCEL_GRAVITY;
            float actualZ = accel.getAccel().z;

            REQUIRE( incline == Approx(0).margin(0.1) );
            REQUIRE( haccel == Approx(z).margin(0.1) );
            REQUIRE( vaccel == Approx(y).margin(0.4) );
            REQUIRE( actualY == Approx(y).margin(0.4) );
            REQUIRE( actualZ == Approx(z).margin(0.1) );
        }
    }
}