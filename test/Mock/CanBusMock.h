#ifndef CAN_BUS_TEST_H_
#define CAN_BUS_TEST_H_

#include "CanBus.h"

/**
 * @brief Pure virtual class which functions as wrapper for MCP2515 Can
 */
class CanBusTest : public CanBus {
	public:
		CanBusTest() { }

		~CanBusTest() { }

		bool readInterruptPin() override { }

		byte checkReceive() override { }

		byte readMsgBuffer(byte* len, byte* buf) { }

		unsigned long getCanId() override { }

		void begin() override { }

		void sendMsgBuffer(unsigned long id, byte ext, byte len, const byte *buf) override {
			// do nothing
		}
};

#endif