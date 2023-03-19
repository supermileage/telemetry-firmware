#include "settings.h"
#include "DebugSerialOutput.h"

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
    Serial.println(s);
}

void DebugSerial::println() { 
    Serial.println();
}

void DebugSerial::print(const char* s) { 
    Serial.print(s);
}

void DebugSerial::printf(const char* format, ...) {
    va_list args;
    Serial.printf(format, args);
}

DebugSerial::DebugSerial() { }