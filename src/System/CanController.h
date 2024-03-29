#ifndef _CAN_CONTROLLER_H_
#define _CAN_CONTROLLER_H_

#include "settings.h"

/**
 * @brief Abstract class which functions as wrapper for can controller library
 */
class CanController {
	public:
		CanController(byte msgAvail) : _messageAvail(msgAvail) { }
		virtual ~CanController() { }
		virtual void begin() = 0;
		virtual bool readInterruptPin() = 0;
		virtual byte checkReceive() = 0;
		virtual byte readMsgBuf(byte* len, byte* buf) = 0;
		virtual unsigned long getCanId() = 0;
		virtual void sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf) = 0;
		inline byte messageAvail() { return _messageAvail; }

	private:
		const byte _messageAvail;
};

#endif