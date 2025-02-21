#ifndef _USART_SERIAL_WRAPPER_HORIZON_H_
#define _USART_SERIAL_WRAPPER_HORIZON_H_

#include "Particle.h"
#include "TelemetrySerial.h"
#include "USARTSerialHorizon.h"

/**
 * @brief Concrete wrapper class for USARTSerial object, implementing the TelemetrySerial interface.
 */
class USARTSerialWrapperHorizon : public TelemetrySerial {
	public:
		USARTSerialWrapperHorizon(USARTSerial *serial);
		~USARTSerialWrapperHorizon();
		void begin(unsigned long baud, uint32_t flags) override;
		int available() override;
		int read() override;
		size_t readBytes(char* buffer, size_t length) override;

	private:
		USARTSerialHorizon* _usartSerial;
};

#endif
