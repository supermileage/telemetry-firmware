#ifndef CAN_BUS_MCP_2515_H_
#define CAN_BUS_MCP_2515_H_

#include "mcp2515_can.h"
#include "CanController.h"
#include "mcp2515_can.h"

/**
 * @brief Concrete mcp2515_can wrapper class
 */
class Mcp2515CanWrapper : public CanController {
	public:
		Mcp2515CanWrapper(SPIClass* spi, uint8_t csPin, uint8_t intPin);
		~Mcp2515CanWrapper() override;
		void begin() override;
		bool readInterruptPin() override;
		byte checkReceive() override;
		byte readMsgBuf(byte* len, byte* buf);
		unsigned long getCanId() override;
		void sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf);

	private:
		uint8_t _intPin;
		mcp2515_can* _mcpCan;
};

#endif