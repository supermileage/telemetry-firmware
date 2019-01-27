#pragma once

/* can-telemetry library by Leslie Cheng
 */

// This will load the definition for common Particle variable types
#include "Particle.h"

// This is your main class that users will import into their application
class CANTelemetry {
public:
    /**
     * Set up the CAN channel for us 
     */
    CANTelemetry(CANChannel &channel, int baud_rate, int timeout, bool debug);
    CANTelemetry(CANChannel &channel, int baud_rate, bool debug);
    CANTelemetry(CANChannel &channel, int baud_rate);

    /**
     * Call and response model. This method will send a CAN message with 
     * the specified header, and return a 64-bit response. Note that the 
     * response may not necessarily be an integer, so be wary. 
     */
    uint64_t poll(uint32_t filter, uint32_t header, int len);

    /**
     * Call and response model. Overloaded function as the one below, where
     * you have the exact same header as the filter. 
     */
    uint64_t poll(uint32_t header, int len);

    uint64_t poll(uint32_t header);

    template <class T>
    T interpret(uint64_t data, int start, int end);

private:
    /**
     * Example private method
     */
    CANChannel * _can;
    int _baud_rate;
    unsigned long _timeout;
    bool _debug;
    uint64_t _decode(uint8_t * arr, int len);
};


template <class T>
T CANTelemetry::interpret(uint64_t data, int start, int end) {
    uint8_t n[8] = {0};                 // Initialize values to 0
    uint8_t * data_cast = reinterpret_cast<uint8_t *>(&data);
    if (start > end) {                  // Big endian
        for (int i = start, a = 0; i >= end; i--, a++) {
            n[a] = data_cast[i];
        }
    } else {                            // Little endian or single byte
        for (int i = start, a = 0; i <= end; i++, a++) {
            n[a] = data_cast[i];
        }
    }
    return *(T *)n;
}