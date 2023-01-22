#ifndef _DRIVER_DISPLAY_H_
#define _DRIVER_DISPLAY_H_

#include <vector>

#include "Adafruit_SH110X.h"
#include "Handleable.h"

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

class DisplayElement {
    public:
        virtual ~DisplayElement() { }
        virtual void draw(Adafruit_SH1107& display) = 0;
        virtual void setPosition(int16_t x, int16_t y) = 0;
};

/* Text element for driver display */
template <typename T, typename R>
class TextElement : public DisplayElement {
    public:
        TextElement(T* obj, R (T::*getter)(bool&), uint8_t textSize, String label = "", uint8_t labelSize = 0) :
            _obj(obj), _getter(getter), _textSize(textSize), _label(label), _labelSize(labelSize) { }

        ~TextElement() { }

        void draw(Adafruit_SH1107& display) override {
            display.setTextColor(SH110X_WHITE);
            display.setCursor(_x, _y);
            
            if (_labelSize != 0) {
                display.setTextSize(_labelSize);

                for (uint16_t i = 0; i < _label.length(); i++) {
                    display.write((uint8_t)_label[i]);
                }
            }
            
            bool dummy;
            String displayString = String((_obj->*_getter)(dummy));
            display.setTextSize(_textSize);

            // add padding
            if (displayString.length() < _minTextLength) {
                for (uint16_t i = 0; i < _minTextLength - displayString.length(); i++) {
                    display.write((uint8_t)_paddingCharacter);
                }
            }
            for (uint16_t i = 0; i < displayString.length(); i++) {
                display.write((uint8_t)displayString[i]);
            }
        }

        void setPosition(int16_t x, int16_t y) {
            _x = x;
            _y = y;
        }

        void setMinTextLength(uint8_t len) {
            _minTextLength = len;
        }

        void setPaddingCharacter(char c) {
            _paddingCharacter = c;
        }

    private:
        T* _obj = nullptr;
        R (T::*_getter)(bool&);
        int16_t _x = 0;
        int16_t _y = 0;
        uint8_t _textSize = 0;
        uint8_t _minTextLength = 0;
        char _paddingCharacter = '0';
        String _label;
        uint8_t _labelSize = 0;
};

#endif