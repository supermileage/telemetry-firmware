#ifndef CAN_BUS_H_
#define CAN_BUS_H_

#include "settings.h"

/**
 * @brief Pure virtual class which functions as wrapper for MCP2515 Can
 */
class CanBus {
	public:
		CanBus(byte msgAvail) : _messageAvail(msgAvail) { }

		virtual ~CanBus() { }

		virtual bool readInterruptPin() = 0;

		virtual byte checkReceive() = 0;

		virtual byte readMsgBuf(byte* len, byte* buf) = 0;

		virtual unsigned long getCanId() = 0;

		virtual void begin() = 0;

		virtual void sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf) = 0;

		inline byte messageAvail() { return _messageAvail; }

	private:
		const byte _messageAvail;
};

#endif