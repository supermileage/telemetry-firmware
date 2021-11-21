#include "Button.h"

#define DEBOUNCE_TIME 50

Button::Button(uint16_t pin, bool activeHigh, void (*callbackPosEdge)() = NULL, void (*callbackNegEdge)() = NULL){
    _pin = pin;
    _activeHigh = activeHigh;
    _callbackPosEdge = callbackPosEdge;
    _callbackNegEdge = callbackNegEdge;

    // Enable input pin
    if(_activeHigh){
        pinMode(pin, INPUT_PULLDOWN);
    }else{
        pinMode(pin, INPUT_PULLUP);
    }
}

bool Button::handle(){
    bool newState = digitalRead(_pin);
    if(_activeHigh){
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