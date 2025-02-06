#include "test_config.h"
#include "TestHelpers.h"

#include "CanInterface.h"
#include "CanControllerMock.h"
#include "CanSensorSteering.h"

TEST_CASE( "CanSensorSteering::getHumanName", "[CanSensorSteering]" ) {
	CanInterface interface(nullptr);
	CanSensorSteering steering(interface);

	REQUIRE( steering.getHumanName().equals("CanSensorSteering") );
}

TEST_CASE( "CanSensorSteering::update", "[CanSensorSteering][Sensor]" ) {
	CanControllerMock canBusMock(CAN_MESSAGE_AVAIL_TEST);
	CanInterface interface(&canBusMock);
	CanSensorSteering steering(interface);
	CanMessage msg = CAN_MESSAGE_NULL;
	setMillis(DEFAULT_START_TIME_MILLIS);

	Handler::instance().begin();

	SECTION( "update throttle -- full range" ) {
		msg.id = CAN_STEERING_THROTTLE;
		msg.dataLength = 1;
		
		for (uint16_t i = 0; i <= 255; i += 15) {
			uint8_t expected = (i * 100) / 255;

			msg.data[0] = i;
			canBusMock.setCanMessage(msg);

			Handler::instance().handle();

			REQUIRE( steering.getThrottle() == expected );
		}

		testValidation(steering, &CanSensorSteering::getThrottle);
	}

	SECTION( "update ignition -- ready" ) {
		msg.id = CAN_STEERING_READY;
		msg.dataLength = 1;

		// set ignition to 1
		msg.data[0] = 0xFF & (1 << STEERING_READY_BIT_IGNITION);
		canBusMock.setCanMessage(msg);

		REQUIRE( steering.getIgnition() == Sensor::Status::Unknown );
		
		Handler::instance().handle();

		REQUIRE( steering.getIgnition() == Sensor::Status::On );

		// set back to 0
		msg.data[0] = 0x0;
		canBusMock.setCanMessage(msg);
		
		Handler::instance().handle();

		REQUIRE( steering.getIgnition() == Sensor::Status::Off );

		testValidation(steering, &CanSensorSteering::getIgnition);
	}

	SECTION( "update dms -- ready" ) {
		msg.id = CAN_STEERING_READY;
		msg.dataLength = 1;

		// set ignition to 1
		msg.data[0] = 0xFF & (1 << STEERING_READY_BIT_DMS);
		canBusMock.setCanMessage(msg);

		REQUIRE( steering.getDms() == Sensor::Status::Unknown );
		
		Handler::instance().handle();

		REQUIRE( steering.getDms() == Sensor::Status::On );

		// set back to 0
		msg.data[0] = 0x0;
		canBusMock.setCanMessage(msg);
		
		Handler::instance().handle();

		REQUIRE( steering.getDms() == Sensor::Status::Off );

		testValidation(steering, &CanSensorSteering::getDms);
	}

	SECTION( "update brake -- ready" ) {
		msg.id = CAN_STEERING_READY;
		msg.dataLength = 1;

		// set ignition to 1
		msg.data[0] = 0xFF & (1 << STEERING_READY_BIT_BRAKE);
		canBusMock.setCanMessage(msg);

		REQUIRE( steering.getBrake() == Sensor::Status::Unknown );
		
		Handler::instance().handle();

		REQUIRE( steering.getBrake() == Sensor::Status::On );

		// set back to 0
		msg.data[0] = 0x0;
		canBusMock.setCanMessage(msg);
		
		Handler::instance().handle();

		REQUIRE( steering.getBrake() == Sensor::Status::Off );

		testValidation(steering, &CanSensorSteering::getBrake);
	}

	SECTION( "update all 3 ready variables" ) {
		msg.id = CAN_STEERING_READY;
		msg.dataLength = 1;

		// set ignition to 1
		msg.data[0] = 0x0;
		msg.data[0] = msg.data[0] | (1 << STEERING_READY_BIT_IGNITION);
		msg.data[0] = msg.data[0] | (1 << STEERING_READY_BIT_DMS);
		msg.data[0] = msg.data[0] | (1 << STEERING_READY_BIT_BRAKE);
		canBusMock.setCanMessage(msg);

		REQUIRE( steering.getIgnition() == Sensor::Status::Unknown );
		REQUIRE( steering.getDms() == Sensor::Status::Unknown );
		REQUIRE( steering.getBrake() == Sensor::Status::Unknown );
		
		Handler::instance().handle();

		REQUIRE( steering.getIgnition() == Sensor::Status::On );
		REQUIRE( steering.getDms() == Sensor::Status::On );
		REQUIRE( steering.getBrake() == Sensor::Status::On );

		// set back to 0
		msg.data[0] = 0x0;
		canBusMock.setCanMessage(msg);
		
		Handler::instance().handle();

		REQUIRE( steering.getIgnition() == Sensor::Status::Off );
		REQUIRE( steering.getDms() == Sensor::Status::Off );
		REQUIRE( steering.getBrake() == Sensor::Status::Off );

		testValidation(steering, &CanSensorSteering::getBrake);
	}
}