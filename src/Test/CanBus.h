#ifndef CAN_BUS_H_
#define CAN_BUS_H_

/**
 * @brief Pure virtual class which functions as wrapper for MCP2515 Can
 */
class CanBus {
	public:
		CanBus() { }

		virtual ~CanBus() { }

		virtual bool readInterruptPin() = 0;

		virtual byte checkReceive() = 0;

		virtual byte readMsgBuffer(byte* len, byte* buf) = 0;

		virtual unsigned long getCanId() = 0;

		virtual void begin() = 0;

		virtual void sendMsgBuffer(unsigned long id, byte ext, byte len, const byte *buf) = 0;
};

#endif