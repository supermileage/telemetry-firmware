#ifndef USART_SERIAL_HORIZON_H
#define USART_SERIAL_HORIZON_H

#include "Particle.h"

class USARTSerialHorizon {
    public:
        USARTSerialHorizon();
        ~USARTSerialHorizon();
    
        void begin(unsigned long baud, uint32_t flags);
        int available();
        int read();
        size_t readBytes(char* buffer, size_t length);
    
    private:
        USARTSerialHorizon _serial;
    };
    
    #endif // USART_SERIAL_HORIZON_H
