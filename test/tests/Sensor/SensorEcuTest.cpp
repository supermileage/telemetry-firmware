#include "test_config.h"

#include <array>
#include <limits>

#include "SensorEcu.h"
#include "TelemetrySerialMock.h"

#define OFFSET_RPM			0x06
#define OFFSET_MAP			0x08
#define OFFSET_TPS			0x0A
#define OFFSET_ECT			0x0C
#define OFFSET_IAT			0x0E
#define OFFSET_O2S			0x10
#define OFFSET_SPARK		0x12
#define OFFSET_FUELPW1		0x14
#define OFFSET_FUELPW2		0x16
#define OFFSET_UBADC		0x18

/* Helper Functions */
void packHeader(uint8_t* buf);
uint8_t getCheckSum(uint8_t* buf);
void setCheckSum(uint8_t* buf);
void packValue(uint8_t* buf, float value, float factor, float offset);
bool packOverflow(float val, float factor);
void testGetterWithinRangeFloat(SensorEcu& ecu, String (SensorEcu::*getter)(bool&), TelemetrySerialMock& serialMock, uint8_t* buf, int32_t index, float min, float max, float factor, float offset, float precision);
void testGetterWithinRangeInt(SensorEcu& ecu, int (SensorEcu::*getter)(bool&), TelemetrySerialMock& serialMock, uint8_t* buf, int32_t index, int32_t min, int32_t max, float factor, float offset);

// ordered header bytes
const std::array<uint8_t, 5> HeaderBytes = {
	SensorEcu::Header1, SensorEcu::Header2, SensorEcu::Header3, SensorEcu::DataFieldLength, SensorEcu::ServiceId
};

/* Tests */
TEST_CASE( "SensorEcu::getHumanName test", "[SensorEcu][Sensor]" ) {
	SensorEcu ecu(nullptr);

	REQUIRE( ecu.getHumanName() == "ECU" );
}

TEST_CASE( "SensorEcu::begin -- calls _serial->begin", "[SensorEcu][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorEcu ecu(&serialMock);

	bool beginCalled = false;

	serialMock.setBegin([&beginCalled](unsigned long baud, uint32_t flags) {
		beginCalled = true;
		REQUIRE( flags == SERIAL_8N1 );
	});

	ecu.begin();

	REQUIRE( beginCalled );
}

TEST_CASE( "SensorEcu::handle -- buffer header / checksum test", "[SensorEcu][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorEcu ecu(&serialMock);
	uint8_t* buf = new uint8_t[SensorEcu::PacketSize]();

	SECTION( "_serial->available() check fails then passes" ) {
		// set up failing check
		bool availableCalled = false;
		bool passedAvailableCheck = true;
		serialMock.setAvailable([&availableCalled, &passedAvailableCheck]() {
			// prevents infinite loop in case SensorEcu::flush is called
			if (availableCalled)
				return 0;
			else
				availableCalled = true;
			
			passedAvailableCheck = false;
			return SensorEcu::PacketSize - 1;
		});
		serialMock.setReadBytes([&passedAvailableCheck](char* buffer, size_t length) {
			passedAvailableCheck = true;
			return 0;
		});
		
		// act
		ecu.handle();

		// assert check has failed
		REQUIRE( availableCalled );
		REQUIRE_FALSE( passedAvailableCheck );


		// set up passing check
		availableCalled = false;
		serialMock.setAvailable([&availableCalled, &passedAvailableCheck]() {
			// prevents infinite loop in case SensorEcu::flush is called
			if (availableCalled)
				return 0;
			else
				availableCalled = true;

			passedAvailableCheck = false;
			return SensorEcu::PacketSize;
		});

		// act
		ecu.handle();

		// assert check has passed
		REQUIRE( availableCalled );
		REQUIRE( passedAvailableCheck );
	}

	SECTION( "Header check fails for any single incorrect byte" ) {
		packHeader(buf);

		// set up readBytes to copy from buf
		serialMock.setReadMessage(buf, SensorEcu::PacketSize);

		for (size_t i = 0; i < HeaderBytes.size(); i++) {
			// set single header byte so it is the wrong value
			buf[i] = ~HeaderBytes[i];

			bool availableCalled = false;
			bool flushCalled = false;
			serialMock.setAvailable([&availableCalled, &flushCalled]() {
				if (availableCalled) {
					flushCalled = true;
					return 0;
				} else {
					availableCalled = true;
				}
				
				return SensorEcu::PacketSize;
			});

			// act
			ecu.handle();

			// assert
			REQUIRE( flushCalled );

			// reset header value in buffer
			buf[i] = HeaderBytes[i];
		}
	}

	SECTION( "Header is valid but checksum fails" ) {
		packHeader(buf);

		serialMock.setReadMessage(buf, SensorEcu::PacketSize);

		// set up wrong checksum
		buf[SensorEcu::PacketSize - 1] = ~getCheckSum(buf);

		// set millis to return time past stale interval
		setMillis(DEFAULT_STALE_TIME_MILLIS);

		// act
		ecu.handle();

		// assert that properties have not been updated and are invalid
		bool valid = false;
		ecu.getRPM(valid);

		REQUIRE( !valid );
	}

	SECTION( "Header is valid and checksum passes" ) {
		packHeader(buf);

		serialMock.setReadMessage(buf, SensorEcu::PacketSize);

		// set up correct checksum
		buf[SensorEcu::PacketSize - 1] = getCheckSum(buf);

		// set millis to return time past stale interval
		setMillis(DEFAULT_STALE_TIME_MILLIS);

		// act
		ecu.handle();

		// assert that properties have been updated and are valid
		bool valid = false;
		ecu.getRPM(valid);
		REQUIRE( valid );
	}

	delete[] buf;
}

TEST_CASE( "SensorEcu::handle -- validation test", "[SensorEcu][Sensor]" ) {
	TelemetrySerialMock serialMock;
	SensorEcu ecu(&serialMock);
	uint8_t* buf = new uint8_t[SensorEcu::PacketSize]();

	// set header and checksum so SensorEcu::_valid will be set to true
	packHeader(buf);
	setCheckSum(buf);
	serialMock.setReadMessage(buf, SensorEcu::PacketSize);
	setMillis(DEFAULT_START_TIME_MILLIS);

	ecu.handle();

	// assert: SensorEcu::_valid should be true from valid update message
	REQUIRE( ecu.getOn() );

	// Edge Cases
	// SensorEcu data is still valid
	setMillis(DEFAULT_STALE_TIME_MILLIS - 1);
	serialMock.setAvailable([]() { return 0; });

	ecu.handle();

	// assert: SensorEcu was't updated but time is still valid
	REQUIRE( ecu.getOn() );

	//SensorEcu data is no longer valid
	setMillis(DEFAULT_STALE_TIME_MILLIS);

	ecu.handle();

	// assert: SensorEcu was't updated and time is now invalid
	REQUIRE_FALSE( ecu.getOn() );

	delete[] buf;
}

TEST_CASE( "SensorEcu::_interpretValue parses buffer correctly for all properties", "[SensorEcu][Sensor][PropertyGetter]" ) {
	TelemetrySerialMock serialMock;
	SensorEcu ecu(&serialMock);
	uint8_t* buf = new uint8_t[SensorEcu::PacketSize]();

	packHeader(buf);
	setMillis(DEFAULT_START_TIME_MILLIS);

	SECTION( "SensorEcu::getRpm" ) {
		testGetterWithinRangeInt(ecu, &SensorEcu::getRPM, serialMock, buf, OFFSET_RPM, 0, 10000, 0.25, 0);
	}

	SECTION( "SensorEcu::getMap" ) {
		testGetterWithinRangeFloat(ecu, &SensorEcu::getMap, serialMock, buf, OFFSET_MAP, 0, 100, 0.0039, 0, 0.01);
	}

	SECTION( "SensorEcu::getTPS" ) {
		testGetterWithinRangeInt(ecu, &SensorEcu::getTPS, serialMock, buf, OFFSET_TPS, 0, 100, 0.0015, 0);
	}

	SECTION( "SensorEcu::getECT" ) {
		testGetterWithinRangeInt(ecu, &SensorEcu::getECT, serialMock, buf, OFFSET_ECT, -10, 100, 1, -40.0);
	}

	SECTION( "SensorEcu::getIAT" ) {
		testGetterWithinRangeInt(ecu, &SensorEcu::getIAT, serialMock, buf, OFFSET_IAT, -10, 100, 1, -40.0);
	}

	// float
	SECTION( "SensorEcu::getO2S" ) {
		testGetterWithinRangeFloat(ecu, &SensorEcu::getO2S, serialMock, buf, OFFSET_O2S, 0, 5, 0.0048, 0, 0.01);
	}

	// int
	SECTION( "SensorEcu::getSpark" ) {
		testGetterWithinRangeInt(ecu, &SensorEcu::getSpark, serialMock, buf, OFFSET_SPARK, 0, 360, 0.5, 0);
	}

	// float
	SECTION( "SensorEcu::getFuelPW1" ) {
		testGetterWithinRangeFloat(ecu, &SensorEcu::getFuelPW1, serialMock, buf, OFFSET_FUELPW1, 0, 60, 0.001, 0, 0.001);
	}

	// float
	SECTION( "SensorEcu::getFuelPW2" ) {
		testGetterWithinRangeFloat(ecu, &SensorEcu::getFuelPW2, serialMock, buf, OFFSET_FUELPW2, 0, 60, 0.001, 0, 0.001);
	}

	// float
	SECTION( "SensorEcu::getUbAdc") {
		testGetterWithinRangeFloat(ecu, &SensorEcu::getUbAdc, serialMock, buf, OFFSET_UBADC, 0, 14, 0.00625, 0, 0.1);
	}

	delete[] buf;
}

void packHeader(uint8_t* buf) {
	for (size_t i = 0; i < HeaderBytes.size(); i++)
		buf[i] = HeaderBytes[i];
}

uint8_t getCheckSum(uint8_t* buf) {
	uint8_t checkSum = 0;
	for(int32_t i = 0; i < SensorEcu::PacketSize - 1; i++)
		checkSum += buf[i];
	
	return checkSum;
}

void setCheckSum(uint8_t* buf) {
	uint8_t checkSum = 0;
	for(int i = 0; i < SensorEcu::PacketSize - 1; i++)
		checkSum += buf[i];
	
	buf[SensorEcu::PacketSize - 1] = checkSum;
}

void packValue(uint8_t* buf, float value, float factor, float offset) {
	int32_t intValue = (value - offset) / factor;
	*buf = (uint8_t)(intValue >> 8);
	*(buf + 1) = (uint8_t)(intValue & 0xFF);
}

bool packOverflow(float val, float factor) {
	return (int32_t)(val / factor - 1) > std::numeric_limits<uint16_t>::max();
}

// set and test 10 different float values over provided range for a SensorEcu property
void testGetterWithinRangeFloat(SensorEcu& ecu, String (SensorEcu::*getter)(bool&), TelemetrySerialMock& serialMock, uint8_t* buf, int32_t index, float min, float max, float factor, float offset, float precision) {
	float increment = (max - min) / 10;
	float val = min;

	for (int32_t i = 0; i < 10; i++) {
		if (packOverflow(i, factor)) {
			std::cout << "Pack overflow!" << std::endl;
			break;
		}


		val = val + (float)rand() / RAND_MAX;

		packValue(buf + index, val, factor, offset);
		setCheckSum(buf);
		serialMock.setReadMessage(buf, SensorEcu::PacketSize);

		ecu.handle();

		bool isValid = false;
		REQUIRE( (ecu.*getter)(isValid).toFloat() == Approx(val).margin(precision) );
		REQUIRE( isValid );

		val += increment;
	}
}

// set and test 10 different int values over provided range for a SensorEcu property
void testGetterWithinRangeInt(SensorEcu& ecu, int (SensorEcu::*getter)(bool&), TelemetrySerialMock& serialMock, uint8_t* buf, int32_t index, int32_t min, int32_t max, float factor, float offset) {
	int32_t increment = (max - min) / 10;

	for (int32_t i = min; i <= max; i+=increment) {
		if (packOverflow(i, factor))
			break;

		packValue(buf + index, (float)i, factor, offset);
		setCheckSum(buf);
		serialMock.setReadMessage(buf, SensorEcu::PacketSize);

		ecu.handle();

		bool isValid = false;
		REQUIRE( (ecu.*getter)(isValid) == i );
		REQUIRE( isValid );
	}
}