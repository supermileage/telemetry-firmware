#ifndef CAN_BUS_MCP_2515_H_
#define CAN_BUS_MCP_2515_H_

#include "mcp2515_can.h"
#include "CanBus.h"
#include "can_common.h"

/**
 * @brief Pure virtual class which functions as wrapper for MCP2515 Can
 */
class CanBusMcp2515 : public CanBus {
	public:
		CanBusMcp2515(SPIClass* spi, uint8_t csPin, uint8_t intPin) : CanBus(CAN_MSGAVAIL) {
			pinMode(intPin, INPUT);
    		_intPin = intPin;
    		_mcpCan = new mcp2515_can(csPin);
    		_mcpCan->setSPI(spi);
		}

		~CanBusMcp2515() override { }

		bool readInterruptPin() override {
			return digitalRead(_intPin);
		}

		byte checkReceive() override {
			return _mcpCan->checkReceive();
		}

		byte readMsgBuf(byte* len, byte* buf) override {
			return _mcpCan->readMsgBuf(len, buf);
		}

		unsigned long getCanId() override {
			return _mcpCan->getCanId();
		}

		void begin() override {
			_mcpCan->begin(CAN_500KBPS,MCP_8MHz);
		}

		void sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf) {
			_mcpCan->sendMsgBuf(id, ext, len, buf);
		}

	private:
		uint8_t _intPin;
        mcp2515_can* _mcpCan;
};

#endif