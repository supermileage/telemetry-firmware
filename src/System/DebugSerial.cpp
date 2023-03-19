#include "settings.h"
#include "DebugSerial.h"

DebugSerial* DebugSerial::_instance = nullptr;
bool DebugSerial::_silent = !DEBUG_SERIAL_EN;

DebugSerial::~DebugSerial() {

}

DebugSerial& DebugSerial::instance() {
    if (_instance == nullptr) {
        _instance = new DebugSerial();
    }
    return *_instance;
}

bool DebugSerial::getSilent() {
    return _silent;
}

void DebugSerial::setSilent(bool silent) { 
    _silent = silent;
}

void DebugSerial::println(const char* s) {
    if (!_silent)
        Serial.println(s);
}

void DebugSerial::println() {
    if (!_silent)
        Serial.println();
}

void DebugSerial::print(const char* s) {
    if (!_silent)
        Serial.print(s);
}

void DebugSerial::printf(const char* format, ...) {
    if (!_silent) {
        va_list args;
        Serial.printf(format, args);
    }
}

DebugSerial::DebugSerial() { }