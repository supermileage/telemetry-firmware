#include "Button.h"

#define DEBOUNCE_TIME 50
#define HOLD_TIME 2000

Button::Button(uint16_t pin, bool activeHigh, bool normallyOpen, void (*callbackPushed)(), void (*callbackReleased)(), void (*callbackHolding)()){
    _pin = pin;
    _activeHigh = activeHigh;
    _callbackPushed = callbackPushed;
    _callbackReleased = callbackReleased;
    _callbackHolding = callbackHolding;

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

Button::~Button(){}

void Button::begin(){}

void Button::handle(){

    bool newState = pinReadFast(_pin);
    if(!_activeHigh){
        newState = !newState;
    }

    if(newState != _state && millis() > _lastChange + DEBOUNCE_TIME){
        _state = newState;
        _lastChange = millis();
        _holding = true;
        if(newState){
            if(_callbackPushed != NULL) _callbackPushed();
        }else{
            if(_callbackReleased != NULL) _callbackReleased();
        }
    }

    if(_holding) {
        if(millis() < _lastChange + HOLD_TIME) {
            if(!newState) {
                _holding = false;
            }
        } else {
            if(newState) {
                _callbackPushed();
                _callbackHolding();
            }
            _holding = false;
        }
    }
}

bool Button::get() {
    return _state;
}