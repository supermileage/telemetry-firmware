#ifndef CAN_BUS_TEST_H_
#define CAN_BUS_TEST_H_

#include <functional>

#include "CanBus.h"

#define MESSAGE_AVAIL 5

/**
 * @brief Mock class for predefining CAN bus behavior
 */
class CanBusMock : public CanBus {
	public:
		CanBusMock() : CanBus(MESSAGE_AVAIL) { }

		~CanBusMock() { }

		bool readInterruptPin() override { 
			return _readInterruptPin();
		}

		byte checkReceive() override { 
			return _checkReceive();
		}

		byte readMsgBuffer(byte* len, byte* buf) override { 
			return _readMsgBuffer(len, buf);
		}

		unsigned long getCanId() override { 
			return _getCanId();
		}

		void begin() override { }

		void sendMsgBuffer(unsigned long id, byte ext, byte len, const byte *buf) override {
			_sendMsgBuffer(id, ext, len, buf);
		}

		void setReadInterruptPin(std::function<bool(void)> func) {
			_readInterruptPin = func;
		}

		void setCheckReceive(std::function<byte(void)> func) {
			_checkReceive = func;
		}

		void setGetCanId(std::function<uint64_t(void)> func) {
			_getCanId = func;
		}

		void setReadMsgBuffer(std::function<byte(byte*,byte*)> func) {
			_readMsgBuffer = func;
		}

		void setSendMsgBuffer(std::function<void(uint64_t,byte,byte,const byte*)> func) {
			_sendMsgBuffer = func;
		}
	
	private:
		std::function<bool(void)> _readInterruptPin;
		std::function<byte(void)> _checkReceive;
		std::function<uint64_t(void)> _getCanId;
		std::function<byte(byte*,byte*)> _readMsgBuffer;
		std::function<void(uint64_t,byte,byte,const byte*)> _sendMsgBuffer;

};

#endif