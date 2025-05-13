#include "test_config.h"

#include <functional>
#include <array>
#include <string.h>

#include "fcp-common.h"
#include "SensorFcpControl.h"
#include "../Mocks/TelemetrySerialMock.h"

/* Helper Functions */
void packHeaderFc(uint8_t* buf);

/* Tests */
TEST_CASE( "SensorFcpControl::getHumanName test", "[SensorFcpControl][Sensor]" ) {
	SensorFcpControl fcp(nullptr);
	REQUIRE( fcp.getHumanName() == "FCP Control" );
}

TEST_CASE( "SensorFcpControl::begin -- calls _serial->begin", "[SensorFcpControl][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorFcpControl fcp(&serialMock);

	bool beginCalled = false;

	serialMock.setBegin([&beginCalled](unsigned long baud, uint32_t flags) {
		beginCalled = true;
		REQUIRE( baud == FC_BAUD );
		REQUIRE( flags == SERIAL_8N1 );
	});

	fcp.begin();

	REQUIRE( beginCalled );
}

TEST_CASE("SensorFcpControl::handle -- validation test", "[SensorFcpControl][Sensor]") {
    TelemetrySerialMock serialMock;
    SensorFcpControl fcp(&serialMock);
    uint8_t* buf = new uint8_t[8]();

    fcp.begin();
    setMillis(DEFAULT_START_TIME_MILLIS);

    SECTION("should pass -- calling getter within stale interval") {
        serialMock.setReadMessage(buf, 8);
        fcp.handle();

        setMillis(DEFAULT_START_TIME_MILLIS + STALE_INTERVAL - 1);
        fcp.handle();

        bool valid = false;
        fcp.getFuelCellVoltage(valid);

        REQUIRE(valid);
    }

    SECTION("should fail -- calling getter after stale interval") {
        serialMock.setReadMessage(buf, 8);
        fcp.handle();

        setMillis(DEFAULT_START_TIME_MILLIS + STALE_INTERVAL);
        fcp.handle();

        bool valid = true;
        fcp.getFuelCellVoltage(valid);

        REQUIRE_FALSE(valid);
    }

    delete[] buf;
}

TEST_CASE("SensorFcpControl::handle -- parses 8-byte FCP packet correctly", "[SensorFcpControl][Sensor]") {
    TelemetrySerialMock serialMock;
    SensorFcpControl fcp(&serialMock);

    // Construct a test buffer with clear decimal values
    uint8_t buf[8] = {
        10,   // error flag		    No error
        40,   // ambient temp: 		40 * 0.5 = 20.0C
        50,  // FC voltage: 		50 * 0.333 = 16.65C
        30,   // H2 leak:			30 * 0.1 = 3.0C
        50,   // FC temp: 			50 * 0.5 = 25.0C
        1,    // current high byte  1 * 256 + 244 = 500
        244,  // current low byte: 	500 * 0.2 = 100A	// Unrealistic value, just for testing
        120   // battery voltage: 	120 * 0.1 = 12.0V
    };

    // Expected physical values (post-scaling)
    const float expectedAmbientTemp     = 20.0;
    const float expectedFuelCellVoltage = 16.65;
    const float expectedH2LeakVoltage   = 3.0;
    const float expectedFuelCellTemp    = 25.0;
    const float expectedCurrent         = 100.0;
    const float expectedBatteryVoltage  = 12.0;

    fcp.begin();
    setMillis(DEFAULT_START_TIME_MILLIS);
    serialMock.setReadMessage(buf, sizeof(buf));
    fcp.handle();

    bool valid = false;

    REQUIRE(fcp.getErrorFlag(valid).toInt() == 10);
    REQUIRE(valid);

    REQUIRE(fcp.getAmbientTemperature(valid).toFloat() == Approx(expectedAmbientTemp).margin(0.01));
    REQUIRE(valid);

    REQUIRE(fcp.getFuelCellVoltage(valid).toFloat() == Approx(expectedFuelCellVoltage).margin(0.01));
    REQUIRE(valid);

    REQUIRE(fcp.getH2LeakVoltage(valid).toFloat() == Approx(expectedH2LeakVoltage).margin(0.01));
    REQUIRE(valid);

    REQUIRE(fcp.getFuelCellTemperature(valid).toFloat() == Approx(expectedFuelCellTemp).margin(0.01));
    REQUIRE(valid);

    REQUIRE(fcp.getFuelCellCurrent(valid).toFloat() == Approx(expectedCurrent).margin(0.01));
    REQUIRE(valid);

    REQUIRE(fcp.getBatteryVoltage(valid).toFloat() == Approx(expectedBatteryVoltage).margin(0.01));
    REQUIRE(valid);
}

TEST_CASE("SensorFcpControl::handle -- interspersed valid and corrupted packets", "[SensorFcpControl][Sensor]") {
    TelemetrySerialMock serialMock;
    SensorFcpControl fcp(&serialMock);
    fcp.begin();
    setMillis(DEFAULT_START_TIME_MILLIS);

    // Define a valid packet
    uint8_t validPacket[8] = {
        10,   // error flag		    No error
        40,   // ambient temp: 		40 * 0.5 = 20.0C
        50,  // FC voltage: 		50 * 0.333 = 16.65C
        30,   // H2 leak:			30 * 0.1 = 3.0C
        50,   // FC temp: 			50 * 0.5 = 25.0C
        1,    // current high byte  1 * 256 + 244 = 500
        244,  // current low byte: 	500 * 0.2 = 100A	// Unrealistic value, just for testing
        120   // battery voltage: 	120 * 0.1 = 12.0V
    };

    // Define corrupted packets (too short)
    uint8_t corruptedPacket[5] = { 1, 2, 3, 4, 5 };

    SECTION("should only update data on valid packets") {
        // First: corrupt packet
        serialMock.setReadMessage(corruptedPacket, sizeof(corruptedPacket));
        fcp.handle();

        bool valid = true;
        fcp.getBatteryVoltage(valid);
        REQUIRE_FALSE(valid);  // No data yet

        // Then: valid packet
        serialMock.setReadMessage(validPacket, sizeof(validPacket));
        fcp.handle();

        String val = fcp.getBatteryVoltage(valid);
        REQUIRE(valid);
        REQUIRE(val == "12.0");

        // Then: another corrupt packet
        serialMock.setReadMessage(corruptedPacket, sizeof(corruptedPacket));
        fcp.handle();

        // Then: another valid packet
        validPacket[1] = 40;  // Update temp: 40 * 0.5 = 20.0C
        serialMock.setReadMessage(validPacket, sizeof(validPacket));
        fcp.handle();

        String ambientTemp = fcp.getAmbientTemperature(valid);
        REQUIRE(valid);
        REQUIRE(ambientTemp == "20.0");
    }

    SECTION("should preserve last valid data after corrupted packet") {
        // Load valid packet
        serialMock.setReadMessage(validPacket, sizeof(validPacket));
        fcp.handle();

        bool valid = false;
        String fcCurrent = fcp.getFuelCellCurrent(valid);
        REQUIRE(valid);
        REQUIRE(fcCurrent == "100.0");

        // Corrupted data
        serialMock.setReadMessage(corruptedPacket, sizeof(corruptedPacket));
        fcp.handle();

        // Should still return old value as valid
        fcCurrent = fcp.getFuelCellCurrent(valid);
        REQUIRE(valid);
        REQUIRE(fcCurrent == "100.0");
    }
}
