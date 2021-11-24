#include "Button.h"

#define DEBOUNCE_TIME 50

Button::Button(uint16_t pin, bool activeHigh, bool normallyOpen, void (*callbackPushed)() = NULL, void (*callbackReleased)() = NULL){
    _pin = pin;
    _activeHigh = activeHigh;
    _callbackPushed = callbackPushed;
    _callbackReleased = callbackReleased;

    // Enable input pin with a built-in resistor pulling opposite of active
    if(_activeHigh){
        pinMode(pin, INPUT_PULLDOWN);
    }else{
        pinMode(pin, INPUT_PULLUP);
    }

    // If the button is normally closed, reverse the polarity of the read. 
    if(!normallyOpen) {
        _activeHigh = !activeHigh;
    }
}

bool Button::handle(){
    bool newState = pinReadFast(_pin);
    if(!_activeHigh){
        newState = !newState;
    }
    if(newState != _state && millis() > _lastChange + DEBOUNCE_TIME){
        _state = newState;
        _lastChange = millis();
        if(newState){
            if(_callbackPosEdge != NULL) _callbackPosEdge();
        }else{
            if(_callbackNegEdge != NULL) _callbackNegEdge();
        }
    }

    return _state;
}