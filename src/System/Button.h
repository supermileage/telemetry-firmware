#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Particle.h"

class Button {
    public:
        /**
         * Constructor
         * 
         * @param pin The input pin for the button
         * @param activeHigh is true if the button is on when logical high
         * @param callbackPosedge void function to call when the button is pushed down
         * @param callbackNegedge void function to call when the button is released
         * 
         * */
        Button(uint16_t pin, bool activeHigh, void (*callbackPosEdge)(), void (*callbackNegEdge)());

        /**
         * Handles reading the button, should be run as often as possible
         * 
         * @return Current Button State
         * */
        bool handle();


    private:        
        uint16_t _pin;
        bool _activeHigh;

        bool _state = false;
        uint32_t _lastChange = 0;
        void (*_callbackPosEdge)();
        void (*_callbackNegEdge)();
};

#endif
