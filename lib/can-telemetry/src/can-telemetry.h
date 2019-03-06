#pragma once
// Load the definition for common Particle objects
#include "Particle.h"
#include <initializer_list>

// Modes of data gathering
enum POLL_MODE {
    CALL_AND_RESPONSE,
    PASSIVE_POLL
};

class CANTelemetry {
public:
    // Set up the telemetry object
    CANTelemetry(CANChannel &channel, int node_id, int timeout = 1000);

    // Initialize the telemetry module
    void init();

    // Poll methods
    uint64_t poll(uint32_t filter, CANMessage msg);
    uint64_t poll(uint32_t filter); 

    // Takes an existing message and adjusts it 
    void adjust(CANMessage &msg, uint32_t id, bool rtr, uint8_t len = 0, 
                std::initializer_list<uint8_t> nums = {});

    // Checks the error status of the bus
    int heartbeat();

    // Change the timeout of the polling modes
    void change_timeout(int timeout = 1000);

    // Interpret the returned data
    template <class T>
    T interpret(uint64_t data, int starting, int ending);
private:
    CANChannel * _can;
    int _node_id;
    unsigned long _timeout;
    CANMessage _default;

    uint64_t _poll(uint32_t filter, CANMessage msg, POLL_MODE mode);
    uint64_t _decode(uint8_t * arr, int len);
    void _set_mask(int mask);
    void _set_mask();
};

/** 
 * Definitions of templated methods need to be in the header to avoid
 * linking issues, thus the definition is here. This method takes a 
 * 64-bit value, and reinterprets it as whatever sort of value desired.
 * @param data  the data (generally one returned from poll)
 * @param start the starting byte (inclusive)
 * @param end   the ending byte (inclusive)
 * NOTE that this supports little and big-endian byte sequences.
 */
template <class T>
T CANTelemetry::interpret(uint64_t data, int s, int e) {
    uint8_t n[8] = {0};
    uint8_t * data_cast = reinterpret_cast<uint8_t *>(&data);
    if (s > e) {
        for (int i = s, a = 0; i >= e; i--, a++) {
            n[a] = data_cast[i];
        }
    } else {
        for (int i = s, a = 0; i <= e; i++, a++) {
            n[a] = data_cast[i];
        }
    }
    return *(T *)n;
}