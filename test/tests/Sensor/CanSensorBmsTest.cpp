#include "test_config.h"
#include "Particle.h"

#include "CanControllerMock.h"
#include "CanSensorOrionBms.h"


TEST_CASE("Test CanSensorBms::handle", "[CanSensorBms][handle]"){
	CanControllerMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorOrionBms bms(interface);
	bool socMessageSent = false;
	setMillis(0);

	// set up mock to verify message (pass in reference to socMessageSent)
	canBusMock.setSendMsgBuffer([&socMessageSent](uint64_t id, byte ext, byte len, const byte* buf) {
		socMessageSent = (id == CAN_TELEMETRY_BMS_DATA);
	});

	REQUIRE( !bms.getIsAsleep() );

	Handler::instance().begin();

	SECTION("Test CanSensorBms::handle doesn't send soc update before interval") {
		Handler::instance().handle();
		REQUIRE( !socMessageSent );

		setMillis(SOC_UPDATE_INTERVAL);
		Handler::instance().handle();
		REQUIRE( !socMessageSent );
	}

	SECTION("Test CanSensorBms::handle sends soc update after interval") {
		setMillis(SOC_UPDATE_INTERVAL + 1);
		Handler::instance().handle();
		REQUIRE( socMessageSent );

		socMessageSent = false;

		setMillis(2 * (SOC_UPDATE_INTERVAL + 1) - 1);
		Handler::instance().handle();
		REQUIRE( !socMessageSent );

		setMillis(2 * (SOC_UPDATE_INTERVAL + 1));
		Handler::instance().handle();
		REQUIRE( socMessageSent );
	}

	SECTION("Test CanSensorBms::handle doesn't send soc update if isAsleep") {
		bms.setIsAsleep(true);

		Handler::instance().handle();
		REQUIRE( !socMessageSent );

		setMillis(SOC_UPDATE_INTERVAL + 1);
		Handler::instance().handle();
		REQUIRE( !socMessageSent );
	}
}