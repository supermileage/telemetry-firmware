#ifndef _TELEMETRY_SERIAL_H_
#define _TELEMETRY_SERIAL_H_

class TelemetrySerial {
	public:
		virtual ~TelemetrySerial() { }
		virtual void begin(unsigned long baud, uint32_t flags) = 0;
		virtual int available() = 0;
		virtual int read() = 0;
		virtual size_t readBytes(char* buffer, size_t length) = 0;
};

#endif