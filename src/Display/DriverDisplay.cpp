#include "DriverDisplay.h"

DriverDisplay::DriverDisplay(Adafruit_SH1107& display) : _display(display) { }

DriverDisplay::~DriverDisplay() { }

void DriverDisplay::begin() {
    _initialized = _display.begin(0x3C, false);

    if (_initialized) {
        _display.setRotation(1);
        _display.clearDisplay();
        _display.cp437(true);
    }
}

void DriverDisplay::handle() {
    if (_initialized && millis() > _lastRefreshMillis + REFRESH_RATE) {
        _lastRefreshMillis = millis();
        _displayAll();
    }
}

void DriverDisplay::addDisplayElement(DisplayElement* element) {
    _elements.push_back(element);
}

String DriverDisplay::getInitStatus() {
    return (_initialized ? "Success" : "Failure");
}

void DriverDisplay::_displayAll() {
    _display.clearDisplay();
    for (DisplayElement* element : _elements) {
        element->draw(_display);
    }
    _display.display();
}