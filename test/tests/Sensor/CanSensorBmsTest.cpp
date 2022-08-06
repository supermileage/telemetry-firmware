#include "test_config.h"
#include "Particle.h"

#include "CanBusMock.h"
#include "CanSensorOrionBms.h"

#define CAN_RECEIVE_VAL 5

TEST_CASE("Test CanSensorBms::handle", "[CanSensorBms][handle]"){
	CanBusMock canBusMock(CAN_RECEIVE_VAL);
	CanInterface interface(&canBusMock);
	CanSensorBms* bms = new CanSensorOrionBms(interface);
	bool socMessageSent = false;

	Handler::instance().begin();

	// set up mock to verify message
	canBusMock.setSendMsgBuffer([&socMessageSent](uint64_t id, byte ext, byte len, const byte* buf) {
		socMessageSent = (id == CAN_TELEMETRY_BMS_DATA);
	});

	REQUIRE( !bms->getIsAsleep() );

	SECTION("Test CanSensorBms::handle doesn't send soc update before interval") {
		setMillis(0);
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

		setMillis(2 * (SOC_UPDATE_INTERVAL + 1));
		Handler::instance().handle();
		REQUIRE( socMessageSent );
	}
}