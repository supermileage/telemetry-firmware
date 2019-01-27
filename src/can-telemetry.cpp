/* can-telemetry library by Leslie Cheng
 */

#include "can-telemetry.h"
/**
 * Constructor.
 */
CANTelemetry::CANTelemetry(CANChannel &channel, int baud_rate, int timeout, bool debug) {
    _can = &channel;
    _baud_rate = baud_rate;
    _timeout = (unsigned long)timeout;
    _debug = debug;
}

CANTelemetry::CANTelemetry(CANChannel &channel, int baud_rate, bool debug) {
    _can = &channel;
    _baud_rate = baud_rate;
    _timeout = 1000;
    _debug = debug;
}

CANTelemetry::CANTelemetry(CANChannel &channel, int baud_rate) {
    _can = &channel;
    _baud_rate = baud_rate;
    _timeout = 1000;
    _debug = false;
}

uint64_t CANTelemetry::poll(uint32_t filter, uint32_t header, int len) {
    if (_debug) {
        _can->begin(_baud_rate, CAN_TEST_MODE);
    } else {
        _can->begin(_baud_rate);
    }
    
    _can->addFilter(filter, 0x7FF);      // Listen for a particular response

    CANMessage message;                 // Empty CAN message
    message.id = header;                // Header of the packet we want to send
    message.len = 0;

    _can->transmit(message);

    if (_can->errorStatus() == CAN_NO_ERROR) {
        CANMessage response;
        unsigned long t_stamp = millis();
        while (true) {                              // Run until timeout
            if (_can->receive(response)) {           // We got a response
                _can->end();
                return _decode(response.data, len);
            }
            if (millis() - t_stamp >= _timeout) {   // We timed out
                _can->end();
                return -1;
            }
        }
    } else {
        _can->end();
        return -1;
    }
}

uint64_t CANTelemetry::poll(uint32_t header, int len) {
    return poll(header, header, len);
}

uint64_t CANTelemetry::poll(uint32_t header) {
    return poll(header, header, 8);
}

/** 
 * This is the worst code I've ever written. Do a bunch of bitbanging
 * for the array of bytes passed in to return a single variable of type
 * uint64_t.
 */
uint64_t CANTelemetry::_decode(uint8_t * arr, int len) {
    uint8_t n[len] = {0};
    for (int i = 0; i < len; i++) {
        n[i] = arr[i];
    }
    return *(uint64_t *)n;
}