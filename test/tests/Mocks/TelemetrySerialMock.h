#ifndef _TELEMETRY_SERIAL_MOCK_H_
#define _TELEMETRY_SERIAL_MOCK_H_

#include <functional>

#include "TelemetrySerial.h"

class TelemetrySerialMock : public TelemetrySerial {
	public:
		TelemetrySerialMock();
		~TelemetrySerialMock();
		void begin(unsigned long baud, uint32_t flags) override;
		int available() override;
		int read() override;
		size_t readBytes(char* buffer, size_t length) override;
		void setReadMessage(const uint8_t* msg, int len);
		void setBegin(std::function<void(unsigned long, uint32_t)> func);
		void setAvailable(std::function<int(void)> func);
		void setRead(std::function<int(void)> func);
		void setReadBytes(std::function<size_t(char*,size_t)> func);

	private:
		std::function<void(unsigned long, uint32_t)> _begin;
		std::function<int(void)> _available;
		std::function<int(void)> _read;
		std::function<size_t(char*,size_t)> _readBytes;
		bool _availableCalled = false;

};

#endif