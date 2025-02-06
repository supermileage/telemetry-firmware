#ifndef _DISPLAY_ELEMENT_H_
#define _DISPLAY_ELEMENT_H_

#include "Adafruit_GFX.h"

class DisplayElement {
    public:
        virtual ~DisplayElement() { }
        virtual void draw(Adafruit_GFX& display) = 0;
        virtual void setPosition(int16_t x, int16_t y) = 0;
};

/* Text element which writes itself to Adafruit_GFX display */
template <typename T>
class TextElement : public DisplayElement {
    public:
        TextElement(T (*func)(), uint8_t textSize, uint16_t textColour, uint8_t labelSize = 0, String label = "") :
            _displayFunc(func), _textSize(textSize), _textColour(textColour), _labelSize(labelSize), _label(label) { }

        ~TextElement() { }

        void draw(Adafruit_GFX& display) override {
            display.setTextColor(_textColour);
            display.setCursor(_x, _y);
            
            if (_labelSize != 0) {
                display.setTextSize(_labelSize);

                for (uint16_t i = 0; i < _label.length(); i++) {
                    display.write((uint8_t)_label[i]);
                }
            }
            
            String displayString = String(_displayFunc());
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
        T (*_displayFunc)();
        int16_t _x = 0;
        int16_t _y = 0;
        uint8_t _textSize;
        uint8_t _minTextLength = 0;
		uint16_t _textColour;
        uint8_t _labelSize = 0;
		char _paddingCharacter = '0';
		String _label;
};

#endif