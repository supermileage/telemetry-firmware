#include "can-telemetry.h"

/** 
 * Constructor for the telematics object. 
 * @param &channel  CAN channel object
 * @param baud_rate desired baud_rate to listen at 
 * @param node_id   the ID of this telemetry node
 * @param timeout   timeout before returning sentinel values
 * @param debug     debug flag 
 */
CANTelemetry::CANTelemetry(CANChannel &channel, int node_id, int timeout, bool debug) {
    _can = &channel;
    _node_id = node_id;
    _timeout = (unsigned long)timeout;
    _debug = debug;
    // Build the default message with current node ID
    CANMessage _def;
    _def.id = node_id;
    _def.len = 8;
    _default = _def;
}

/**
 * Initialization function. Should be called after the CAN 
 * bus is started. 
 */ 
void CANTelemetry::init() {
    _set_mask();
}

/**
 * Poll using CALL_AND_RESPONSE method.
 * @param filter    response pattern to listen for from the bus
 * @param msg       msg to query the bus with
 */
uint64_t CANTelemetry::poll(uint32_t filter, CANMessage msg) {
    return _poll(filter, msg, CALL_AND_RESPONSE);
}

/**
 * Poll using PASSIVE_POLL method.
 * @param filter    response pattern to listen for from the bus
 */
uint64_t CANTelemetry::poll(uint32_t filter) {
    return _poll(filter, _default, PASSIVE_POLL);
}

/**
 * Creates a new CANMessage object with the given parameters.
 * This is intended to be a helper method so that it's easier to 
 * create messages on the fly for multiple poll options.
 * @param msg   CANMessage object, passed by reference
 * @param id    id to change the message to
 * @param rtr   whether this is a remote frame
 * @param len   length of the data
 * @param nums  initializer list of the data
 */
CANMessage CANTelemetry::createMsg(uint32_t id, bool rtr, 
                          uint8_t len, std::initializer_list<uint8_t> nums) {
    CANMessage msg;
    msg.id = id;
    msg.rtr = rtr;
    msg.len = len;
    int i = 0;
    for (int n : nums) {
        msg.data[i] = n;
        i++;
        // Max length of message
        if (i == 8) break;
    }
    // Set the rest of the values to 0
    while (i != 8) {
        msg.data[i] = 0;
        i++;
    }
    return msg;
}

/**
 * Heartbeat check of the CAN bus. Sends a message with the 
 * specified node ID and tests whether the bus returns an error.
 * Returns 1 for no error, 0 for error. 
 */
int CANTelemetry::heartbeat() {
    _can->transmit(_default);
    if (_can->errorStatus() == CAN_NO_ERROR) return 1;
    else return 0;
}

/**
 * Adjust the timeout for the poll before returning.
 */
void CANTelemetry::change_timeout(int timeout) {
    _timeout = (unsigned long)timeout;
}

/**
 * Poll the CAN bus, and returns the values as an unsigned 64-bit object.
 * So an unsigned long long, basically. This is just a helper method.
 * @param filter    response pattern to listen from the bus
 * @param msg       message to send 
 * @param mode      mode to run at (CALL_AND_RESP or PASSIVE_POLL)
 */
uint64_t CANTelemetry::_poll(uint32_t filter, CANMessage msg, POLL_MODE mode) {

    // Set the mask and transmit our message
    _set_mask(filter);

    // Transmit the message if CALL_AND_RESPONSE specified
    if (mode == CALL_AND_RESPONSE) {
        _can->transmit(msg);
    }

    // Wait for a response, or timeout
    if (_can->errorStatus() == CAN_NO_ERROR) {
        CANMessage response;
        unsigned long t_stamp = millis();
        while (true) {
            if (_can->receive(response)) {
                if (_debug) Serial.println("Received");
                _set_mask();
                return _decode(response.data, response.len);
            }
            if (millis() - t_stamp >= _timeout) {
                if (_debug) Serial.println("Timeout");
                _set_mask();
                return -1;
            }
        }
    } else {
        if (_debug) Serial.println("CAN error");
        heartbeat();
        _set_mask();
        return -1;
    }
}

/** 
 * Decode and return the bytes as an unsigned 64-bit value (unsigned long
 * long). This is a private helper method.
 */
uint64_t CANTelemetry::_decode(uint8_t * arr, int len) {
    uint8_t n[8] = {0};
    for (int i = 0; i < len; i++) {
        n[i] = arr[i];
    }
    return *(uint64_t *)n;
}

/**
 * Set the mask on the CAN stream. It defaults to 0. This is a private
 * helper method.
 */
void CANTelemetry::_set_mask(int mask) {
    _can->clearFilters();
    _can->addFilter(mask, 0x7FF);
}

/**
 * Set the mask on the CAN stream. It defaults to the node ID. Private.
 */
void CANTelemetry::_set_mask() {
    _set_mask(_node_id);
}