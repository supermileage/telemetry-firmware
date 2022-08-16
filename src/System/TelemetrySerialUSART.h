#ifndef _TELEMETRY_SERIAL_USART_H_
#define _TELEMETRY_SERIAL_USART_H_

#include "Particle.h"
#include "TelemetrySerial.h"

class TelemetrySerialUSART : public TelemetrySerial {
	public:
		TelemetrySerialUSART(USARTSerial *serial);
		~TelemetrySerialUSART();
		void begin(unsigned long baud, uint32_t flags) override;
		int available() override;
		int read() override;
		size_t readBytes(char* buffer, size_t length) override;

	private:
		USARTSerial* _serial;
};

#endif