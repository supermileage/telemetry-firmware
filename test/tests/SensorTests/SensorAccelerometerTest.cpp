#include "test_config.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <numeric>

#include <functional>
#include <math.h>

#include "SensorAccelerometer.h"
#include "AccelerometerControllerMock.h"

#define EULERS 2.7182f

/* Tests */
TEST_CASE( "SensorAccelerometer::getHumanName test", "[SensorAccelerometer][Sensor]" ) {
    SensorAccelerometer accel(nullptr, 1000);
    
    REQUIRE( accel.getHumanName() == "Accelerometer" );
}

TEST_CASE( "SensorAccelerometer::begin -- pitch", "[SensorAccelerometer][Sensor][begin]" ) {
    AccelerometerControllerMock mock;
    SensorAccelerometer accel(&mock, 1000);

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

// TEST_CASE( "SensorAccelerometer::handle", "[SensorAccelerometer][Sensor][handle]") {
//     AccelerometerControllerMock mock;
//     SensorAccelerometer accel(&mock, 1000);

//     SECTION("Mock sensor test: filter reduces variance of y and z components") {
//         std::string currentPath = std::filesystem::current_path();
//         size_t idx = currentPath.find("/test/bin");
//         std::string path = currentPath.substr(0, idx) + "/test/tests/data/";
//         std::string inputPath = path + "mock_accelerometer_data.csv";

//         std::ifstream inputFile(inputPath);
//         REQUIRE(inputFile.is_open());

//         std::string line;
//         std::vector<Vec3> samples;
//         samples.reserve(2000);
//         while (std::getline(inputFile, line)) {
//             std::istringstream iss(line);
//             std::string token;

//             Vec3 vec;
//             std::getline(iss, token, ',');
//             vec.x = std::stof(token);
//             std::getline(iss, token, ',');
//             vec.y = std::stof(token);
//             std::getline(iss, token, ',');
//             vec.z = std::stof(token);
//             samples.push_back(vec);
//             if (samples.size() >= 2000) break; // keep memory bounded for CI
//         }

//         setMillis(0);
//         mock.setReturnValues(samples[0], Vec3 { 0,0,0 }, true);
//         accel.begin();

//         std::vector<float> rawY; rawY.reserve(samples.size());
//         std::vector<float> rawZ; rawZ.reserve(samples.size());
//         std::vector<float> filtY; filtY.reserve(samples.size());
//         std::vector<float> filtZ; filtZ.reserve(samples.size());

//         rawY.push_back(samples[0].y);
//         rawZ.push_back(samples[0].z);
//         filtY.push_back(accel.getVerticalAcceleration().toFloat());
//         filtZ.push_back(accel.getHorizontalAcceleration().toFloat());

//         for (unsigned i = 1; i < samples.size(); i++) {
//             const Vec3 sample = samples[i];
//             setMillis(i * ACCEL_READ_INTERVAL);
//             mock.setReturnValues(sample, Vec3 { 0, 0, 0 }, true);
//             accel.handle();
//             rawY.push_back(sample.y);
//             rawZ.push_back(sample.z);
//             filtY.push_back(accel.getVerticalAcceleration().toFloat());
//             filtZ.push_back(accel.getHorizontalAcceleration().toFloat());
//         }

//         auto variance = [](const std::vector<float>& v){
//             if (v.size() < 2) return 0.0f;
//             float mean = std::accumulate(v.begin(), v.end(), 0.0f) / static_cast<float>(v.size());
//             float acc = 0.0f;
//             for (float x : v) {
//                 float d = x - mean;
//                 acc += d * d;
//             }
//             return acc / static_cast<float>(v.size() - 1);
//         };

//         float varRawY = variance(rawY);
//         float varFiltY = variance(filtY);
//         float varRawZ = variance(rawZ);
//         float varFiltZ = variance(filtZ);

//         REQUIRE(varFiltY < varRawY);
//         REQUIRE(varFiltZ < varRawZ);
//     }
// }
