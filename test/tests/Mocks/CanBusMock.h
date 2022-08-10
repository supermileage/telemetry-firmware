#ifndef CAN_BUS_TEST_H_
#define CAN_BUS_TEST_H_

#include <functional>

#include "can.h"
#include "CanBus.h"

using namespace can;

/**
 * @brief Mock class for predefining CAN bus behavior
 */
class CanBusMock : public CanBus {
	public:
		CanBusMock(byte messageAvail);
		~CanBusMock();
		bool readInterruptPin() override;
		byte checkReceive() override ;
		byte readMsgBuf(byte* len, byte* buf) override;
		unsigned long getCanId() override;
		void begin() override;
		void sendMsgBuf(unsigned long id, byte ext, byte len, const byte *buf) override;
		void setReadInterruptPin(std::function<bool(void)> func);
		void setCheckReceive(std::function<byte(void)> func);
		void setGetCanId(std::function<uint64_t(void)> func);
		void setReadMsgBuffer(std::function<byte(byte*,byte*)> func);
		void setSendMsgBuffer(std::function<void(uint64_t,byte,byte,const byte*)> func);
		void setCanMessage(CanMessage msg);
	
	private:
		std::function<bool(void)> _readInterruptPin;
		std::function<byte(void)> _checkReceive;
		std::function<uint64_t(void)> _getCanId;
		std::function<byte(byte*,byte*)> _readMsgBuf;
		std::function<void(uint64_t,byte,byte,const byte*)> _sendMsgBuf;
		bool _checkReceiveCalled = false;

};

#endif