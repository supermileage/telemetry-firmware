#include "test_config.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

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

TEST_CASE( "SensorAccelerometer::handle", "[SensorAccelerometer][Sensor][handle]") {
    AccelerometerControllerMock mock;
    SensorAccelerometer accel(&mock, 1000);

    SECTION("Mock sensor test: smooth over artificial noisy accelerometer data") {
        std::string currentPath = std::filesystem::current_path();
        size_t idx = currentPath.find("/bin");
        std::cout << "current file system path: " << currentPath << std::endl;
        std::string path = currentPath.substr(0, idx) + "/tests/data/";
        std::string inputPath = path + "mock_accelerometer_data.csv";
        std::cout << inputPath << std::endl;

        std::ifstream inputFile(inputPath);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening the file.\n";
            REQUIRE(false);
        }

        std::string line;
        std::vector<Vec3> samples;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string token;

            Vec3 vec;
            std::getline(iss, token, ',');
            vec.x = std::stof(token);
            std::getline(iss, token, ',');
            vec.y = std::stof(token);
            std::getline(iss, token, ',');
            vec.z = std::stof(token);
            samples.push_back(vec);
        }
        std::cout << samples.size() << std::endl;
    

        std::string outputFolderPath = path + "out/";
        std::cout << outputFolderPath << std::endl;
        std::filesystem::create_directories(outputFolderPath);
        std::string outputPath = outputFolderPath + "output.csv"; 
        std::cout << outputPath << std::endl;
        std::ofstream outputFile(outputPath);

        if (!outputFile.is_open()) {
            REQUIRE(false);
        }

        setMillis(0);
        mock.setReturnValues(samples[0], Vec3 { 0,0,0 }, true);
        accel.begin();

        float haccel = accel.getHorizontalAcceleration().toFloat();
        float vaccel = accel.getVerticalAcceleration().toFloat();

        outputFile << haccel << "," << vaccel << std::endl;
        for (unsigned i = 1; i < samples.size(); i++) {
            Vec3 sample = samples[i];
            setMillis(i * ACCEL_READ_INTERVAL);
            mock.setReturnValues(Vec3 { sample.x, sample.y, sample.z }, Vec3 { 0, 0, 0 }, true);

            accel.handle();

            haccel = accel.getHorizontalAcceleration().toFloat();
            vaccel = accel.getVerticalAcceleration().toFloat();
            outputFile << haccel << "," << vaccel << std::endl;
        }
        outputFile.close();

        REQUIRE(true);
    }
}