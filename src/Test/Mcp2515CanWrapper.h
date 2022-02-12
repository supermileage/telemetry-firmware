#ifndef CAN_WRAPPER_H_
#define CAN_WRAPPER_H_

#include "CanBus.h"
#include "can_common.h"

/**
 * @brief Pure virtual class which functions as wrapper for MCP2515 Can
 */
class Mcp2515CanWrapper : public CanBus {
	public:
		Mcp2515CanWrapper(SPIClass* spi, uint8_t csPin, uint8_t intPin) {
			pinMode(intPin, INPUT);
    		_intPin = intPin;
    		_mcpCan = new mcp2515_can(csPin);
    		_mcpCan->setSPI(spi);
		}

		~Mcp2515CanWrapper() override { }

		bool readInterruptPin() override {
			return digitalRead(_intPin);
		}

		byte checkReceive() override {
			return _mcpCan->checkReceive();
		}

		byte readMsgBuffer(byte* len, byte* buf) override {
			return _mcpCan->readMsgBuf(len, buf);
		}

		unsigned long getCanId() override {
			return _mcpCan->getCanId();
		}

		void begin() override {
			_mcpCan->begin(CAN_500KBPS,MCP_8MHz);
		}

		void sendMsgBuffer(unsigned long id, byte ext, byte len, const byte *buf) {
			_mcpCan->sendMsgBuf(id, ext, len, buf);
		}

	private:
		uint8_t _intPin;
        mcp2515_can* _mcpCan;
};

#endif