#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Particle.h"
#include "Handleable.h"

class Button : public Handleable {
    public:
        /**
         * Constructor
         * 
         * @param pin The input pin for the button
         * @param activeHigh is true if the button is active when logical high
         * @param normallyOpen is true if the button is normall open
         * @param callbackPushed void function to call when the button is pushed down
         * @param callbackReleased void function to call when the button is released
         * @param callbackHeld void function to call when the button is held for 2s
         * 
         * */
        Button( uint16_t pin, 
                bool activeHigh, 
                bool normallyOpen, 
                void (*callbackPushed)() = NULL, 
                void (*callbackReleased)() = NULL, 
                void (*callbackHolding)() = NULL);
        ~Button();

        void begin();

        /**
         * Handles reading the button, should be run as often as possible
         * */
        void handle();

        /**
         * Return button status
         * */
        bool get();


    private:        
        uint16_t _pin;
        bool _activeHigh;

        bool _state = false;
        bool _holding = false;
        uint32_t _lastChange = 0;
        void (*_callbackPushed)();
        void (*_callbackReleased)();
        void (*_callbackHolding)();
};

#endif
