#ifndef _DRIVER_DISPLAY_H_
#define _DRIVER_DISPLAY_H_

#include <vector>
#include "Adafruit_SH110X.h"
#include "Handleable.h"
#include "DisplayElement.h"

#define REFRESH_RATE 100

class DisplayElement;

/* Provides functionality for displaying arbitrary text on SH1107 display */
class DriverDisplay : public Handleable {
    public:
        DriverDisplay(Adafruit_SH1107& display);
        ~DriverDisplay();
        void begin() override;
        void handle() override;
        void addDisplayElement(DisplayElement* element);
        String getInitStatus();

    private:
        uint64_t _lastRefreshMillis;
        Adafruit_SH1107& _display;
        std::vector<DisplayElement*> _elements;
        bool _initialized = false;

        void _displayAll();
};

#endif