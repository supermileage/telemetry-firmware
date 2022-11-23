#include "test_config.h"

#include <functional>
#include <array>
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
		REQUIRE( flags == SERIAL_8N1 );
	});

	fcp.begin();

	REQUIRE( beginCalled );
}

TEST_CASE( "SensorFcpControl::handle -- message length tests", "[SensorFcpControl][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorFcpControl fcp(&serialMock);
	uint8_t* buf = new uint8_t[SensorFcpControl::PacketSize]();

	bool readCalled = false;
	
	SECTION( "should fail -- fewer bytes available than message length" ) {

		serialMock.setAvailable([]() {
			return SensorFcpControl::PacketSize - 1;
		});
		serialMock.setReadBytes([readCalled](char* buf, int len) {
			readCalled = true;
		});
		serialMock.setRead([readCalled]() {
			readCalled = true;
		});

		fcp.handle();

		REQUIRE_FALSE( readCalled );
	}

	SECTION( "should pass -- message is correct length and has correct header" ) {
		serialMock.setAvailable([]() {
			return SensorFcpControl::PacketSize;
		});
		serialMock.setReadBytes([readCalled](char* buf, int len) {
			packHeaderFc(buf);
			readCalled = true;
		});
		serialMock.setRead([readCalled]() {
			readCalled = false;
		});

		fcp.handle();

		REQUIRE( readCalled );
	}

	SECTION( "should fail -- message contains invalid header" ) {
		bool flushCalled = false;
		bool availableCalled = false;

		serialMock.setAvailable([availableCalled]() {
			// prevents infinite loop in case SensorEcu::flush is called
			if (availableCalled)
				return 0;
			else
				availableCalled = true;

			return SensorFcpControl::PacketSize;
		});

		serialMock.setReadMessage([](char* buf, int len) {
			packHeaderFc(buf);
			buf[1] = ~FC_HEADER_1;
		});
		serialMock.setRead([flushCalled]() {
			flushCalled = true;
		});

		fcp.handle();

		REQUIRE( readCalled );
	}

}

void packHeaderFc(uint8_t* buf) {
	for (size_t i = 0; i < HeaderBytes.size(); i++)
		buf[i] = HeaderBytes[i];
}