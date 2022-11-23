#include "test_config.h"

#include <functional>
#include <array>
#include <string.h>

#include "fcp-common.h"
#include "SensorFcpControl.h"
#include "TelemetrySerialMock.h"

/* Helper Functions */
void packHeaderFc(uint8_t* buf);

// ordered header bytes
const std::array<uint8_t, 6> HeaderBytes = {
	FC_HEADER_0, FC_HEADER_1, FC_HEADER_2, FC_HEADER_3, FC_HEADER_4, FC_HEADER_5
};

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

TEST_CASE( "SensorFcpControl::handle -- buffer header test", "[SensorFcpControl][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorFcpControl fcp(&serialMock);
	uint8_t* buf = new uint8_t[SensorFcpControl::PacketSize]();

	fcp.begin();
	
	SECTION( "should fail -- fewer bytes available than message length" ) {
		bool readCalled = false;

		serialMock.setAvailable([]() {
			return SensorFcpControl::PacketSize - 1;
		});
		serialMock.setReadBytes([&readCalled](char* buf, int len) {
			readCalled = true;
			return len;
		});
		serialMock.setRead([&readCalled]() {
			readCalled = true;
			return 0;
		});

		fcp.handle();

		REQUIRE_FALSE( readCalled );
	}

	SECTION( "should pass -- message is correct length and has correct header" ) {
		bool readCalled = false;
		bool availableCalled = false;

		packHeaderFc(buf);

		serialMock.setAvailable([&availableCalled]() {
			// prevents infinite loop in case SensorFcpControl::flush is called
			if (availableCalled)
				return 0;
			else
				availableCalled = true;

			return SensorFcpControl::PacketSize;
		});
		serialMock.setReadBytes([&buf, &readCalled](char* buffer, int len) {
			memcpy((void*)buffer, (void*)buf, len);
			readCalled = true;
			return len;
		});

		fcp.handle();

		REQUIRE( availableCalled );
		REQUIRE( readCalled );
	}

	SECTION( "should fail -- message contains invalid header" ) {
		bool flushCalled = false;
		bool readCalled = false;
		int timesAvailableCalled = 0;
		
		packHeaderFc(buf);
		buf[1] = ~FC_HEADER_1;
		
		serialMock.setAvailable([&timesAvailableCalled]() {
			timesAvailableCalled++;
			if (timesAvailableCalled > 5) {
				return 0;
			}
			return SensorFcpControl::PacketSize;
		});
		serialMock.setReadBytes([&buf, &readCalled](char* buffer, int len) {
			memcpy((void*)buffer, (void*)buf, len);
			readCalled = true;
			return len;
		});
		serialMock.setRead([&flushCalled]() {
			flushCalled = true;
			return 0;
		});

		fcp.handle();

		REQUIRE( readCalled );
		REQUIRE( timesAvailableCalled != 0 );
		REQUIRE( flushCalled );
	}

	delete[] buf;
}

TEST_CASE( "SensorFcpControl::handle -- validation test", "[SensorFcpControl][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorFcpControl fcp(&serialMock);
	uint8_t* buf = new uint8_t[SensorFcpControl::PacketSize]();
	packHeaderFc(buf);

	fcp.begin();
	setMillis(DEFAULT_START_TIME_MILLIS);

	SECTION("should pass -- calling getter within stale interval") {
		serialMock.setReadMessage(buf, SensorFcpControl::PacketSize);
		
		// receive message
		fcp.handle();

		setMillis(DEFAULT_START_TIME_MILLIS + STALE_INTERVAL - 1);

		// set data validity
		fcp.handle();

		bool isValid = false;
		fcp.getCellVoltageByIndex(0, isValid);

		REQUIRE( isValid );
	}

	SECTION("should fail -- calling getter after stale interval") {
		serialMock.setReadMessage(buf, SensorFcpControl::PacketSize);
		
		// receive message
		fcp.handle();

		setMillis(DEFAULT_START_TIME_MILLIS + STALE_INTERVAL);

		// set data validity
		fcp.handle();

		bool isValid = false;
		fcp.getCellVoltageByIndex(0, isValid);

		REQUIRE_FALSE( isValid );
	}
	delete[] buf;
}

TEST_CASE( "SensorFcpControl::handle -- message parsing test", "[SensorFcpControl][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorFcpControl fcp(&serialMock);
	uint8_t* buf = new uint8_t[SensorFcpControl::PacketSize]();
	packHeaderFc(buf);

	fcp.begin();
	setMillis(DEFAULT_START_TIME_MILLIS);
	
	SECTION("should pass -- correctly parses a range of cell values") {
		const float cellValues[] = { 0, 0.01, -0.01, 0.125, -0.125, 0.7, -0.7, 0.88, -0.88, 1, -1, 2.55, -2.55, 13.33, -13.33, 25.5, -25.5 };

		REQUIRE( sizeof(cellValues) / sizeof(cellValues[0]) == FC_NUM_CELLS );

		for (int i = 0; i < FC_NUM_CELLS; i++) {
			int16_t toPack = (int16_t)(cellValues[i] * 1000);
			buf[FC_NUM_HEADERS + i * 2] = toPack >> 8;
			buf[FC_NUM_HEADERS + i * 2 + 1] = toPack & 0xFF;
		}

		serialMock.setReadMessage(buf, SensorFcpControl::PacketSize);

		fcp.handle();

		for (int i = 0; i < FC_NUM_CELLS; i++) {
			bool isValid = false;
			float val = fcp.getCellVoltageByIndex(i, isValid);

			REQUIRE( isValid );
			REQUIRE( cellValues[i] == Approx(val).margin(0.01) );
		}
	}
	delete[] buf;
}

void packHeaderFc(uint8_t* buf) {
	for (size_t i = 0; i < HeaderBytes.size(); i++)
		buf[i] = HeaderBytes[i];
}