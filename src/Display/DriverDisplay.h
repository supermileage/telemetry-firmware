#ifndef _DRIVER_DISPLAY_H_
#define _DRIVER_DISPLAY_H_

#include <vector>

#include "Adafruit_SH110X.h"
#include "Handleable.h"

#define REFRESH_RATE 100

class DisplayElement {
    public:
        DisplayElement() { }
        virtual ~DisplayElement() { }
        virtual void draw(Adafruit_SH1107& display) = 0;
        virtual void setPosition(int16_t x, int16_t y) = 0;
};

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
            for (uint16_t i = 0; i < displayString.length(); i++) {
                display.write((uint8_t)displayString[i]);
            }
        }

        void setPosition(int16_t x, int16_t y) {
            _x = x;
            _y = y;
        }

    private:
        T* _obj = nullptr;
        R (T::*_getter)(bool&);
        int16_t _x = 0;
        int16_t _y = 0;
        uint8_t _textSize = 0;
        String _label;
        uint8_t _labelSize = 0;
};

class DriverDisplay : public Handleable {
    public:
        DriverDisplay(Adafruit_SH1107& display) : _display(display) { }
        
        ~DriverDisplay() { }

        void begin() override {
            _initialized = _display.begin(0x3C, false);

            if (_initialized) {
                _display.setRotation(1);
                _display.clearDisplay();
                _display.cp437(true);
            }
        }

        void handle() override {
            if (_initialized && millis() > _lastRefreshMillis + REFRESH_RATE) {
                _lastRefreshMillis = millis();
                _displayAll();
            }
        }

        void addDisplayElement(DisplayElement* element) {
            _elements.push_back(element);
        }

        String getInitStatus() {
            return (_initialized ? "Success" : "Failure");
        }

    private:
        uint64_t _lastRefreshMillis;
        Adafruit_SH1107& _display;
        std::vector<DisplayElement*> _elements;
        bool _initialized = false;

        void _displayAll() {
            _display.clearDisplay();
            for (DisplayElement* element : _elements) {
                element->draw(_display);
            }
            _display.display();
        }
};

#endif